pub mod traits;
pub mod builtins;
pub mod variabledef;
mod typedefs;



use crate::errors::{error::Error, GenaricError };
use crate::Scope;
use crate::Context;
use crate::shared::{postion::Postion, result::IResult};
use crate::parser::Node;
use traits::BuiltInFunc;
use std::rc::Rc;
use std::cell::RefCell;
use std::convert::TryInto;
use std::collections::hash_set::HashSet;
use typedefs::*;

use colored::*;

#[derive(Debug,Clone,PartialEq)]
pub enum Varible {
    Integer(Integer),
    Float(f32),
    String(String),
    List(Vec<Varible>),
    Enum(Enum),
    EnumValue(EnumValue),
    BuiltIn(BuiltIn),
    Function(Function),
    Namespace(Namespace),
    Object(Object),
    Return(Box<Varible>),
    Null,
    Break,
    Continue
}
impl Varible {
    pub fn new_builtin<T>(name: T, args: Vec<String>, run: Box<dyn BuiltInFunc>) -> Varible  where T: ToString {
        Varible::BuiltIn(BuiltIn {
            name: name.to_string(),
            func_args: args,
            run,
            start: Postion::default(),
            end: Postion::default(),
        })
    }
    pub fn new_function(name: String, args: Vec<String>, body: Node, start: Postion, end: Postion) -> Varible {
        Varible::Function(Function {
            name,
            body,
            start,
            end,
            func_args: args
        })
    }
    pub fn new_namespace(name: String, parent: Rc<Context>, start: Postion) -> Namespace {

        let mut scope = Scope::new();

        scope.set_parent(parent.as_ref().scope.clone()).expect("Namespace: Failed to set scope parent");

        Namespace {
            name: name.clone(),
            context: Rc::new(
                Context::new(
                format!("<namespace {}>",name.clone()).to_string(), 
                parent.file_name.clone(), 
                Rc::new(RefCell::new(scope)), 
                Some(parent.clone()),
                Some(start)))
        }
    }
    pub fn new_enum(name: String, items: Vec<String>) -> Self {
        let mut keys = HashSet::new();
        for key in items {
            keys.insert(key);
        }
        Varible::Enum(Enum { name, keys })
    }
    pub fn new_integer(value: isize) -> Self {
        Varible::Integer(Integer {value, boolean: false})
    }
    pub fn new_bool(value: bool) -> Self {
        Varible::Integer(Integer {value: value as isize, boolean: true})
    }
    pub fn new_object(props: &Vec<(String,Varible)>) -> Self {
           Varible::Object(Object::new(props))
    }
    pub fn cast(&self, _target: &str) -> IResult<Varible, Box<dyn Error>, Varible> {
        unimplemented!();
    }
    pub fn power(&self, other: &Self) -> IResult<Varible, Box<dyn Error>, Varible> {
        match self {
            Varible::Integer(value) => {
                if let Varible::Integer(ot) = other {
                    IResult::Ok(Varible::new_integer(value.value.pow(ot.value.try_into().unwrap())))
                } else {
                    return IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not add to different types")))
                }
            }
            Varible::Float(value) => {
                if let Varible::Float(ot) = other {
                    IResult::Ok(Varible::Float(value.powf(*ot))) 
                }else {
                    return IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not add to different types")))
                }
            }
            _ => IResult::Err(Box::new(GenaricError::com("OptionationError", "operation not supported")))
        }
    }
    pub fn add(&self, other: &Self) -> IResult<Varible, Box<dyn Error>, Varible> {
        match self {
            Varible::Integer(value) => {
                if let Varible::Integer(ot) = other {
                    IResult::Ok(Varible::new_integer(value.value + ot.value))
                }else{
                    IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not add to different types")))
                }
            }
            Varible::Float(value) => {
                if let Varible::Float(ot) = other {
                    IResult::Ok(Varible::Float(value + ot)) 
                }else {
                    return IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not add to different types")))
                }
            }
            Varible::String(value) => {
                if let Varible::String(ot) = other {
                    IResult::Ok(Varible::String(format!("{}{}",value,ot).to_string()))
                }else {
                    return IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not add to different types")))
                }
            }
            _ => IResult::Err(Box::new(GenaricError::com("OptionationError", "operation not supported")))
        }
    }
    pub fn sub(&self, other: &Self) -> IResult<Varible, Box<dyn Error>, Varible> {
        match self {
            Varible::Integer(value) => {
                if let Varible::Integer(ot) = other {
                    IResult::Ok(Varible::new_integer(value.value - ot.value))
                }else{
                    IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not add to different types")))
                }
            }
            Varible::Float(value) => {
                if let Varible::Float(ot) = other {
                    IResult::Ok(Varible::Float(value - ot))
                }else{
                    IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not add to different types")))
                }
            }
            _ => IResult::Err(Box::new(GenaricError::com("OptionationError", "operation not supported")))
        }
    }
    pub fn div(&self, other: &Self) -> IResult<Varible, Box<dyn Error>, Varible> {
        match self {
            Varible::Integer(value) => {
                if let Varible::Integer(ot) = other {
                    if ot.value == 0 {
                        return IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not divide by 0")))
                    }
                    IResult::Ok(Varible::new_integer(value.value / ot.value))
                }else{
                    IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not add to different types")))
                }
            }
            Varible::Float(value) => {
                if let Varible::Float(ot) = other {
                    if ot <= &0.0 && ot >= &0.0 {
                        return IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not divide by 0")))
                    }
                    IResult::Ok(Varible::Float(value / ot))
                }else{
                    IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not add to different types")))
                }
            }
            _ => IResult::Err(Box::new(GenaricError::com("OptionationError", "operation not supported")))
        }
    }
    pub fn mul(&self, other: &Self) -> IResult<Varible, Box<dyn Error>, Varible> {
        match self {
            Varible::Integer(value) => {
                if let Varible::Integer(ot) = other {
                    IResult::Ok(Varible::new_integer(value.value * ot.value))
                }else{
                    IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not add to different types")))
                }
            }
            Varible::Float(value) => {
                if let Varible::Float(ot) = other {
                    IResult::Ok(Varible::Float(value * ot))
                }else{
                    IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not add to different types")))
                }
            }
            _ => IResult::Err(Box::new(GenaricError::com("OptionationError", "operation not supported")))
        }
    }
    pub fn comp(&self, other: &Self, not: bool) ->  IResult<Varible, Box<dyn Error>, Varible> {
        if not {
            IResult::Ok(Varible::new_bool(self != other))
        }else{
            IResult:: Ok(Varible::new_bool(self == other))
        }
    }
    pub fn gt(&self, other: &Self) -> IResult<Varible, Box<dyn Error>, Varible> { 
        match self {
            Varible::Integer(value) => {
                if let Varible::Integer(ot) = other {
                    IResult::Ok(Varible::new_bool(value.value > ot.value))
                }else{
                    IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not add to different types")))
                }
            }
            Varible::Float(value) => {
                if let Varible::Float(ot) = other {
                    IResult::Ok(Varible::new_bool(value > ot))
                }else{
                    IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not add to different types")))
                }
            }
            _ => IResult::Err(Box::new(GenaricError::com("OptionationError", "operation not supported")))
        }
    }
    pub fn gte(&self, other: &Self) -> IResult<Varible, Box<dyn Error>, Varible> { 
        match self {
            Varible::Integer(value) => {
                if let Varible::Integer(ot) = other {
                    IResult::Ok(Varible::new_bool(value.value >= ot.value))
                }else{
                    IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not add to different types")))
                }
            }
            Varible::Float(value) => {
                if let Varible::Float(ot) = other {
                    IResult::Ok(Varible::new_bool(value >= ot))
                }else{
                    IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not add to different types")))
                }
            }
            _ => IResult::Err(Box::new(GenaricError::com("OptionationError", "operation not supported")))
        }
    }
    pub fn lt(&self, other: &Self) -> IResult<Varible, Box<dyn Error>, Varible> {
        match self {
            Varible::Integer(value) => {
                if let Varible::Integer(ot) = other {
                    IResult::Ok(Varible::new_bool(value.value < ot.value))
                }else{
                    IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not add to different types")))
                }
            }
            Varible::Float(value) => {
                if let Varible::Float(ot) = other {
                    IResult::Ok(Varible::new_bool(value < ot))
                }else{
                    IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not add to different types")))
                }
            }
            _ => IResult::Err(Box::new(GenaricError::com("OptionationError", "operation not supported")))
        }
    }
    pub fn lte(&self, other: &Self) -> IResult<Varible, Box<dyn Error>, Varible> { 
        match self {
            Varible::Integer(value) => {
                if let Varible::Integer(ot) = other {
                    IResult::Ok(Varible::new_bool(value.value <= ot.value))
                }else{
                    IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not add to different types")))
                }
            }
            Varible::Float(value) => {
                if let Varible::Float(ot) = other {
                    IResult::Ok(Varible::new_bool(value <= ot))
                }else{
                    IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not add to different types")))
                }
            }
            _ => IResult::Err(Box::new(GenaricError::com("OptionationError", "operation not supported")))
        }
    }
    pub fn and(&self, other: &Self) -> IResult<Varible, Box<dyn Error>, Varible> { 
        match self {
            Varible::Integer(value) => {
                if let Varible::Integer(ot) = other {
                    IResult::Ok(Varible::new_bool((value.value != 0) && (ot.value != 0)))
                }else{
                    IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not add to different types")))
                }
            }
            _ => IResult::Err(Box::new(GenaricError::com("OptionationError", "operation not supported")))
        }
    }
    pub fn or(&self, other: &Self) -> IResult<Varible, Box<dyn Error>, Varible> { 
        match self {
            Varible::Integer(value) => {
                if let Varible::Integer(ot) = other {
                    IResult::Ok(Varible::new_bool((value.value != 0) || (ot.value != 0)))
                }else{
                    IResult::Err(Box::new(GenaricError::com("OptionationError", "Can not add to different types")))
                }
            }
            _ => IResult::Err(Box::new(GenaricError::com("OptionationError", "operation not supported")))
        }
    }
    pub fn is_true(&self) -> bool {
        match self {
            Varible::Integer(value) => {
                value.value != 0
            }
            Varible::Float(value) => {
                value >= &0.0 && value <= &0.0 
            }
            Varible::String(value) => {
                !value.is_empty()
            }
            Varible::List(value) => {
                !value.is_empty()
            }
            Varible::EnumValue(_) | Varible::Function(_) | Varible::Object(_) | Varible::BuiltIn(_) => true,
            _ => false
        }
    }
}
impl std::fmt::Display for Varible {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Varible::List(value) => {
                let mut lists = String::from("[ ");
                for elem in value {
                    lists.push_str(format!("{}, ",elem).as_str());
                }
                lists.push_str(" ]");
                write!(f, "{}",lists)
            }
            Varible::Integer(value) => write!(f, "{}", value),
            Varible::Float(value) => write!(f, "{}", value),
            Varible::String(value) => write!(f, "{}",format!("\"{}\"", value).to_string().magenta()),
            Varible::Null => write!(f, "{}","null".truecolor(109, 109, 109)),
            Varible::Object(value) => write!(f, "{}", value),
            Varible::Enum(value) => write!(f,"{}",value),
            Varible::EnumValue(value) => write!(f,"{}",value),
            Varible::Namespace(value) => write!(f,"{}",value),
            Varible::Function(value) => write!(f,"{}",value),
            a => {
                println!("{:#?}", a);
                write!(f, "{}","<undefined>".truecolor(109, 109, 109))
            }
        }
    }
}