use std::collections::HashMap;
use async_trait::async_trait;
use tokio::fs::{ File };
use tokio::io::{AsyncWriteExt};
use crate::interpreter::{ Functional, VisitResult, DataType, Property };
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


pub fn insert_fs(data: &mut HashMap<String,Property>) {
    data.insert("writeFile".into(), Property { 
        value: WriteFile::new(),
        writable: false
    });
}