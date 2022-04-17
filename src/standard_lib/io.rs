use std::collections::HashMap;

use crate::interpreter::{ Functional, VisitResult, DataType, Property };
use crate::errors::RuntimeError;

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

impl Functional for PrintLn {
    fn execute(&self, args: Vec<crate::interpreter::DataType>) -> crate::interpreter::VisitResult<crate::interpreter::DataType> {
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


pub fn insert_io(data: &mut HashMap<String,Property>) {
    data.insert("printLn".into(), Property { 
        value: PrintLn::new(),
        writable: false
     });
}