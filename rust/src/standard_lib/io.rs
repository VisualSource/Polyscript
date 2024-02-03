use std::collections::HashMap;
use std::io::stdin;
use async_trait::async_trait;
use crate::interpreter::{ Functional, VisitResult, DataType, Property };
use crate::errors::RuntimeError;

#[derive(Debug,Clone)]
struct ReadLine {}

impl ReadLine {
    fn new() -> DataType {
        DataType::NativeFunction { 
            name: "readLine".into(), 
            args: vec![], 
            method: Box::new(Self {}) 
        }
    } 
}

#[async_trait]
impl Functional for ReadLine {
    async fn execute(&self, _args: Vec<crate::interpreter::DataType>) -> crate::interpreter::VisitResult<crate::interpreter::DataType> {

        let mut buf = String::default();

        if let Err(err) = stdin().read_line(&mut buf) {
            return VisitResult::Error(RuntimeError::StdError { module: "io", error: err })
        }

        VisitResult::Ok(DataType::STRING { value: buf.trim_end().to_string() })
    }
}

#[derive(Debug,Clone)]
struct PrintLn {}

impl PrintLn {
    fn new() -> DataType {
        DataType::NativeFunction { 
            name: "printLn".into(), 
            args: vec!["out".into()], 
            method: Box::new(Self {}) 
        }
    } 
}
#[async_trait]
impl Functional for PrintLn {
    async fn execute(&self, args: Vec<crate::interpreter::DataType>) -> crate::interpreter::VisitResult<crate::interpreter::DataType> {
        match args.get(0) {
            Some(value) => {
                println!("{}",value);
                VisitResult::Ok(DataType::NULL)
            },
            None => VisitResult::Error(RuntimeError::SyntaxError { 
                reason: "invaild arguments".into()
            })
        }
    }
}

#[derive(Debug,Clone)]
struct Print {}

impl Print {
    fn new() -> DataType {
        DataType::NativeFunction { 
            name: "print".into(), 
            args: vec!["out".into()], 
            method: Box::new(Self {}) 
        }
    } 
}
#[async_trait]
impl Functional for Print {
    async fn execute(&self, args: Vec<crate::interpreter::DataType>) -> crate::interpreter::VisitResult<crate::interpreter::DataType> {
        match args.get(0) {
            Some(value) => {
                print!("{}",value);
                VisitResult::Ok(DataType::NULL)
            },
            None => VisitResult::Error(RuntimeError::SyntaxError { 
                reason: "invaild arguments".into()
            })
        }
    }
}


pub fn insert_io(data: &mut HashMap<String,Property>) {
    data.insert("println".into(), Property { 
        value: PrintLn::new(),
        writable: false
    });
    data.insert("print".into(), Property { 
        value: Print::new(),
        writable: false
    });
    data.insert("readLine".into(), Property { 
        value: ReadLine::new(),
        writable: false
    });
}