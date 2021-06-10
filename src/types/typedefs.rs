use super::traits::*;
use super::Varible;

use crate::errors::{error::Error, GenaricError };
use crate::Scope;
use crate::Context;
use crate::shared::{postion::Postion, result::IResult };
use crate::parser::Node;
use crate::interpreter::Interpreter;
use std::rc::Rc;
use std::cell::RefCell;
use std::collections::hash_map::HashMap;
use std::collections::hash_set::HashSet;
use std::fmt::Display;

use colored::*;

#[derive(Debug,Clone)]
pub struct Integer {
    pub value: isize,
    pub boolean: bool
}
impl PartialEq for Integer {
    fn eq(&self, other: &Self) -> bool {
        self.value == other.value
    }
}

impl Display for Integer {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        if self.boolean {
            write!(f, "{}", format!("{}",self.value != 0).to_string().red())
        }else{
            write!(f, "{}", format!("{}",self.value).to_string().bright_green())
        }   
    }
}

#[derive(Debug,Clone)]
pub struct EnumValue {
    pub parent: String,
    pub value: String
}

impl PartialEq for EnumValue {
    fn eq(&self, other: &Self) -> bool {
        (self.parent == other.parent) && (self.value == other.value)
    }
} 

impl Display for EnumValue {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f,"{}",format!("<enum {}::{}>",self.parent.bright_cyan(),self.value.bright_green()).to_string().truecolor(109, 109, 109))  
    }
}

#[derive(Debug,Clone,PartialEq)]
pub struct Enum {
    pub name: String,
    pub keys: HashSet<String>
} 
 
impl Enum {
    pub fn get(&self, key: &String) -> IResult<Varible, Box<dyn Error>, Varible> {
        match self.keys.get(key) {
            None => IResult::Err(Box::new(GenaricError::com("RuntimeError", "key does not exist in enum"))),
            Some(value) =>  IResult::Ok(Varible::EnumValue(EnumValue { value: value.to_owned(), parent: self.name.clone() })) 
        }
    }
}

impl Display for Enum {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f,"{}",format!("<enum {}>",self.name.bright_cyan()).to_string().truecolor(109, 109, 109))
    }
}

#[derive(Debug,Clone)]
pub struct Namespace {
    pub name: String,
    pub context: Rc<Context>
}
impl PartialEq for Namespace {
    fn eq(&self, _other: &Self) -> bool { false }
}

impl Display for Namespace {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f,"<Namespace {}>",self.name)
    }
}

#[derive(Debug,Clone)]
pub struct Object {
    properties: HashMap<String,Varible>
}
impl Object {
    pub fn new(props: &Vec<(String,Varible)>) -> Self {
        let mut properties = HashMap::new();
        for el in props {
            properties.insert(el.0.clone(), el.1.clone());
        }
        Self {
            properties: properties
        }
    }
    pub fn get(&self, key: &String) -> Result<Varible,Box<dyn Error>> {
        match self.properties.get(key) {
            Some(value) => {
                Ok(value.to_owned())
            }
            None => Err(Box::new(GenaricError::com("RuntimeError","key is undefined")))
        }
    }
    
}

impl std::fmt::Display for Object {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let mut returns = String::from("{ ");
        for (key, prop) in self.properties.iter() {
            returns.push_str(&format!("{}: {},",key,prop).to_string())
        }
        returns.push_str(" }");

        write!(f,"{}",returns)
    }
}

impl PartialEq for Object {
    fn eq(&self, _other: &Self) -> bool { false }
}

#[derive(Debug,Clone)]
pub struct BuiltIn {
    pub name: String,
    pub end: Postion,
    pub start: Postion,
    pub func_args: Vec<String>,
    pub run: Box<dyn BuiltInFunc>
}
impl Postional for BuiltIn {
    fn get_start(&self) -> Postion {
        self.start
    }
    fn get_end(&self) -> Postion {
        self.end
    }
}

impl Functional for BuiltIn {
    fn exceute(&self, args: Vec<Varible>, parent_ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {
        let mut scope = Scope::new();
        
        match scope.set_parent(parent_ctx.as_ref().scope.clone()) {
            Err(err) => return IResult::Err(err),
            Ok(_) => {}
        }
        let ctx = Rc::new(
            Context::new(
                format!(
                    "<Funciton {}>",self.get_name()), 
                    parent_ctx.file_name.clone(), 
                    Rc::new(
                        RefCell::new(
                            scope
                        )
                    ), 
                   Some(parent_ctx.clone()),
                   Some(self.get_start()) 
                )
            );

        match self.check_and_populate_args(&self.func_args, &args, ctx.clone()) {
            Err(err) => return IResult::Err(err),
            Ok(_) => {}
        }

        self.run.exceute(args, ctx.clone())     
    }
    fn get_name(&self) -> String {
        self.name.clone()
    }
}

impl PartialEq for BuiltIn {
    fn eq(&self, _other: &Self) -> bool { false }
}

impl Display for BuiltIn {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f,"{}",format!("<function {}>",self.name.bright_cyan()).to_string().truecolor(109, 109, 109))
    }
}

#[derive(Debug,Clone)]
pub struct Function {
    pub name: String,
    pub start: Postion,
    pub end: Postion,
    pub body: Node,
    pub func_args: Vec<String>,
}

impl Postional for Function {
    fn get_start(&self) -> Postion {
        self.start
    }
    fn get_end(&self) -> Postion {
        self.end
    }
}

impl Functional for Function {
    fn exceute(&self, args: Vec<Varible>, parent_ctx: Rc<Context>)-> IResult<Varible, Box<dyn Error>, Varible> {

        let mut scope = Scope::new();
        
        match scope.set_parent(parent_ctx.as_ref().scope.clone()) {
            Err(err) => return IResult::Err(err),
            Ok(_) => {}
        }

        let ctx = Rc::new(
                    Context::new(
                        format!(
                            "<Funciton {}>",self.get_name()), 
                            parent_ctx.file_name.clone(), 
                            Rc::new(
                                RefCell::new(
                                 scope  
                                )
                            ), 
                           Some(parent_ctx.clone()),
                           Some(self.get_start()) 
                        )
                    );
            
        match self.check_and_populate_args(&self.func_args, &args, ctx.clone()) {
            Err(err) => return IResult::Err(err),
            Ok(_) => {}
        }


        let inter = Interpreter::new();
        inter.visit(self.body.clone(), ctx.clone())
    }
    fn get_name(&self) -> String {
        self.name.clone()
    }
}

impl PartialEq for Function {
    fn eq(&self, _other: &Self) -> bool { false }
}

impl Display for Function {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f,"{}",format!("<function {}>",self.name.bright_cyan()).to_string().truecolor(109, 109, 109))
    }
}



