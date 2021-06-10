pub mod error;

use std::rc::Rc;
use error::Error;
use crate::shared::postion::Postion;
use crate::shared::context::Context;

#[derive(Default)]
pub struct GenaricError {
    start: Postion,
    end: Postion,
    details: String,
    name: String
}

impl GenaricError {
    pub fn com<T>(name: T, details: T) -> Self where T: ToString {
        Self {
            end: Postion::default(),
            start: Postion::default(),
            name: name.to_string(),
            details: details.to_string()
        }
    }
}

impl Error for GenaricError {
    fn get_name(&self) -> String {
        self.name.clone()
    }
    fn get_start(&self) -> Postion {
        self.start.clone()
    }
    fn get_end(&self) -> Postion {
        self.end.clone()
    }
    fn get_details(&self) -> String {
        self.details.clone()
    }
    fn to_string(&self) -> String {
        let pos = self.get_start();
        format!("{}: '{}' at Ln {}:{}",self.get_name(),self.details,pos.ln, pos.col).to_string()
    }
}

#[derive(Debug)]
pub struct ExpectedCharError {
    start: Postion,
    end: Postion,
    details: String
}

impl ExpectedCharError {
    pub fn new(details: String, start: Postion, end: Postion) -> Self {
        Self {
            details,
            start,
            end
        }
    }
}

impl Error for ExpectedCharError {
    fn get_name(&self) -> String {
        "ExpectedCharError".to_string()
    }
    fn get_start(&self) -> Postion {
        self.start.clone()
    }
    fn get_end(&self) -> Postion {
        self.end.clone()
    }
    fn get_details(&self) -> String {
        self.details.clone()
    }
    fn to_string(&self) -> String {
        let pos = self.get_start();
        format!("{}: '{}' at Ln {}:{}",self.get_name(),self.details,pos.ln, pos.col).to_string()
    }
}

#[derive(Debug)]
pub struct IllegalCharError {
    start: Postion,
    end: Postion,
    details: String
}
impl IllegalCharError {
    pub fn new(details: String, start: Postion, end: Postion) -> Self {
        Self {
            details,
            start,
            end
        }
    }
}

impl Error for IllegalCharError {
    fn get_name(&self) -> String {
        "IllegalCharError".to_string()
    }
    fn get_start(&self) -> Postion {
        self.start.clone()
    }
    fn get_end(&self) -> Postion {
        self.end.clone()
    }
    fn get_details(&self) -> String {
        self.details.clone()
    }
    fn to_string(&self) -> String {
        let pos = self.get_start();
        format!("{}: '{}' at Ln {}:{}",self.get_name(),self.details,pos.ln, pos.col).to_string()
    }
}


pub struct RuntimeError {
    start: Postion,
    end: Postion,
    details: String,
    context: Option<Rc<Context>>
}
impl RuntimeError {
    pub fn new(details: String, context: Option<Rc<Context>>, start: Postion, end: Postion) -> Self {
        Self {
            details,
            start,
            end,
            context
        }
    }
}
impl Error for RuntimeError {
    fn get_name(&self) -> String {
        "Runtime Error".to_string()
    }
    fn get_start(&self) -> Postion {
        self.start.clone()
    }
    fn get_end(&self) -> Postion {
        self.end.clone()
    }
    fn get_details(&self) -> String {
        self.details.clone()
    }
    fn to_string(&self) -> String {
        let mut error = String::from("Traceback (most recent call last):\n");

        let mut pos = self.start.clone();
        let mut ctx: Option<Rc<Context>> = self.context.clone();

        while ctx.is_some() {
            if let Some(d) = ctx {
                error.push_str(format!("  File {}, line {}, in {}\n",d.file_name.clone(),pos.ln + 1, d.display_name).as_str());
                pos = d.parent_entry;
                ctx = d.parent.clone();
            }
        }

        error.push_str(format!("{}: {}",self.get_name(),self.get_details()).as_str());
        error
    }
}



pub struct InvaildSyntaxError {
    start: Postion,
    end: Postion,
    details: String
}

impl InvaildSyntaxError {
    pub fn new(details: String, start: Postion, end: Postion) -> Self {
        Self {
            details,
            start,
            end
        }
    }
}

impl Error for InvaildSyntaxError {
    fn get_name(&self) -> String {
        "InvaildSyntaxError".to_string()
    }
    fn get_start(&self) -> Postion {
        self.start.clone()
    }
    fn get_end(&self) -> Postion {
        self.end.clone()
    }
    fn get_details(&self) -> String {
        self.details.clone()
    }
    fn to_string(&self) -> String {
        let pos = self.get_start();
        let end = self.get_end();
        format!("{}: '{}' at Ln {}:{} to Ln {}:{}",self.get_name(),self.details,pos.ln, pos.col, end.ln,end.col).to_string()
    }
}