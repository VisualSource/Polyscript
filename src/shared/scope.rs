use std::rc::Rc;
use std::collections::HashMap;
use std::cell::RefCell;
use crate::types::Varible;
use crate::errors::{ error::Error, RuntimeError, GenaricError };
use crate::shared::{postion::Postion, result::IResult};

use super::context::Context;

#[derive(Debug)]
struct ScopeProperty {
    writable: bool,
    value: Varible
}
impl ScopeProperty {
    fn new(value: Varible, writable: bool) -> Self {
        Self {
            value,
            writable
        }
    }
}
#[derive(Debug)]
pub struct Scope {
    pub parent: Option<Rc<RefCell<Self>>>,
    table: HashMap<String,ScopeProperty>
}

impl Scope {
    pub fn new() -> Self {
        Self {
            parent: None,
            table: HashMap::new()
        }
    }
    pub fn set_parent(&mut self, parent: Rc<RefCell<Scope>>) -> Result<(),Box<dyn Error>> {
        if self.parent.is_some() {
            Err(Box::new(GenaricError::com("ScopeError", "Parent is already set.")))
        }else{
            self.parent = Some(parent.clone());
            Ok(())
        } 

    }
    /*pub fn is_empty(&self) -> bool {
        self.table.is_empty()
    }*/
    pub fn has_key<T>(&self, key: &T) -> bool where T: ToString {
        self.table.contains_key(&key.to_string())
    }
    pub fn set<T>(&mut self, key: &T, value: Varible, context: Option<Rc<Context>>) -> IResult<Varible, Box<dyn Error>, Varible>  where T: ToString {
        match self.table.get_mut(&key.to_string()) {
            None => {
                if let Some(parent) = &self.parent {
                    parent.borrow_mut().set(key,value,context)
                }else {
                    println!("{:#?}",self.table);
                    IResult::Err(Box::new(RuntimeError::new(format!("{} is undefined",key.to_string()).to_string(),context,Postion::default(),Postion::default())))
                }
            }
            Some(prop) => {
                if !prop.writable {
                    IResult::Err(Box::new(RuntimeError::new(format!("{} can not be writen to",key.to_string()).to_string(),context,Postion::default(),Postion::default())))
                }else {
                    prop.value = value.clone();
                    IResult::Ok(prop.value.to_owned())
                }
            }
        }
    }
    pub fn get<T>(&self, key: &T, context: Option<Rc<Context>>) -> IResult<Varible, Box<dyn Error>, Varible>  where T: ToString {
        match self.table.get(&key.to_string()) {
            None => {
                if let Some(parent) = &self.parent {
                    parent.as_ref().borrow().get(key,context)
                }else{
                    IResult::Err(Box::new(RuntimeError::new(format!("{} is undefined",key.to_string()).to_string(),context,Postion::default(),Postion::default())))
                }
            }
            Some(prop) => {
                IResult::Ok(prop.value.to_owned())
            }
        }
    }
    pub fn insert<T>(&mut self, key: &T, value: Varible, writable: bool , context: Option<Rc<Context>>) -> IResult<Varible, Box<dyn Error>, Varible> where T: ToString {
        
        if self.has_key(key) {
            return IResult::Err(Box::new(RuntimeError::new(format!("{} is already declared",key.to_string()).to_string(),context,Postion::default(),Postion::default())));
        }

        self.table.insert(key.to_string(), ScopeProperty::new(value.clone(), writable));

        IResult::Ok(value)
    }
}

#[cfg(test)]
mod tests {
   // use super::*;
    #[test]
    fn scope_var_set() {
        //let var_name = String::from("name");
        //let mut scope = Scope::new();
        
        //scope.insert(&var_name, Varible::Null, true, None).expect("variable was set");
        
    }
}