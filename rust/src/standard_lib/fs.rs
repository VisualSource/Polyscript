use std::collections::HashMap;
use std::path::PathBuf;
use std::sync::{Arc, Mutex};
use async_trait::async_trait;
use tokio::fs::{ File, read_to_string };
use tokio::io::{AsyncWriteExt};
use crate::interpreter::{ Functional, VisitResult, DataType, Property, Ctx, Context };
use crate::errors::RuntimeError;

#[derive(Debug,Clone)]
struct WriteFile {}

impl WriteFile {
    fn new() -> DataType {
        DataType::NativeFunction { 
            name: "writeFile".into(), 
            args: vec!["file".into(), "content".into()], 
            method: Box::new(Self {}) 
        }
    } 
}

#[async_trait]
impl Functional for WriteFile {
    async fn execute(&self, args: Vec<crate::interpreter::DataType>) -> crate::interpreter::VisitResult<crate::interpreter::DataType> {

        let filepath = match args.get(0) {
            Some(raw) => {
                if let DataType::STRING { value } = raw {
                    value.to_owned()
                } else {
                    return VisitResult::Error(RuntimeError::TypeError { reason: "Expected argument 1 to be a string".into() })
                }
            },
            None => return VisitResult::Error(RuntimeError::SyntaxError { reason: "Expected 2 arguments".into() })
        };

        let content = match args.get(1) {
            Some(raw) => {
                if let DataType::STRING { value } = raw {
                    value.to_owned()
                } else {
                    return VisitResult::Error(RuntimeError::TypeError { reason: "Expected argument 1 to be a string".into() })
                }
            }
            None => return VisitResult::Error(RuntimeError::SyntaxError { reason: "Expected 2 arguments".into() })
        };

        match File::create(filepath).await {
            Ok(mut value) => {
               match value.write(content.as_bytes()).await {
                   Ok(size) => VisitResult::Ok(DataType::INTEGER { value: size as isize }),
                   Err(err) => return VisitResult::Error(RuntimeError::StdError { module: "fs", error: err })
               }
            }
            Err(err) => return VisitResult::Error(RuntimeError::StdError { module: "fs", error: err })
        }
    }
}

#[derive(Debug,Clone)]
struct ReadFile {}

impl ReadFile {
    pub fn new() -> DataType {
        DataType::NativeFunction { 
            name: "readFile".into(), 
            args: vec!["file".into()], 
            method: Box::new(Self {}) 
        }
    } 
}
#[async_trait]
impl Functional for ReadFile {
    async fn execute(&self, args: Vec<DataType>) -> VisitResult<DataType> {
        let filepath = match args.get(0) {
            Some(raw) => {
                if let DataType::STRING { value } = raw {
                    value.to_owned()
                } else {
                    return VisitResult::Error(RuntimeError::TypeError { reason: "Expected argument 1 to be a string".into() })
                }
            },
            None => return VisitResult::Error(RuntimeError::SyntaxError { reason: "Expected 1 argument".into() })
        };


        let content = match read_to_string(filepath).await {
            Ok(value)=> value,
            Err(err) => return VisitResult::Error(RuntimeError::StdError { module: "fs.readFile", error: err } )
        };

        VisitResult::Ok(DataType::STRING { value: content })
    }
}


pub fn import_fs(ctx: &Ctx) -> DataType {
    let mut scope: HashMap<String,Property> = HashMap::new();
    scope.insert("writeFile".into(), Property { 
        value: WriteFile::new(),
        writable: false
    });
    scope.insert("readFile".into(),Property { writable: false, value: ReadFile::new() });


    let ns = Arc::new(Mutex::new(
        Context::with(Some(ctx.clone()),"fs".into(),PathBuf::from("INTERNAL/fs"),scope)
    ));

    DataType::NAMESPACE(ns)
}