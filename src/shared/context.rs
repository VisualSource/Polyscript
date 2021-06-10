use std::rc::Rc;
use std::cell::{RefCell};

use crate::shared::{postion::Postion,scope::Scope};

#[derive(Debug)]
pub struct Context {
    pub parent_entry: Postion,
    pub file_name: String,
    pub display_name: String,
    pub parent: Option<Rc<Self>>,
    pub scope: Rc<RefCell<Scope>>
}
impl Context {
    pub fn new<T>(display_name: T, file_name: T, scope: Rc<RefCell<Scope>>, parent: Option<Rc<Self>>, parent_entry: Option<Postion>) -> Self where T: ToString {
        Self {
           display_name: display_name.to_string(),
           file_name: file_name.to_string(),
           scope,
           parent,
           parent_entry: parent_entry.unwrap_or(Postion::default())
        }
    }
}