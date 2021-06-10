extern crate dyn_clone;
use crate::shared::{postion::Postion, result::IResult};
use crate::errors::{error::Error, RuntimeError};
use crate::Context;
use std::rc::Rc;

use super::Varible;

use dyn_clone::DynClone;

/// Holds the postion of the current start and end of node in a given file
/// # Example 
/// ```  
/// use crate::types::Postional;
/// impl Position for Trait {
///  fn get_start(&self) -> Postion {
///      self.start.clone()
///  }
///  fn get_end(&self) -> Postion {
///      self.end.clone()
///  }
/// }
pub trait Postional {
    fn get_start(&self) -> Postion;
    fn get_end(&self) -> Postion;
}

// Defines a built in function
pub trait BuiltInFunc: DynClone {
    fn exceute(&self, args: Vec<Varible>, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible>;
}

dyn_clone::clone_trait_object!(BuiltInFunc);

impl std::fmt::Debug for dyn BuiltInFunc {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("BuiltIn Function").finish()
    }
}
// defines a function
pub trait Functional: Postional {
    fn exceute(&self, args: Vec<Varible>, parent_ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible>;
    fn get_name(&self) -> String;
    fn check_and_populate_args(&self, arg_names: &Vec<String>, args: &Vec<Varible>, func_ctx: Rc<Context>) -> Result<(),Box<dyn Error>> {

        if args.len() > arg_names.len() {
            return Err(Box::new(RuntimeError::new("Too marny arguments".to_string(), Some(func_ctx.clone()), self.get_start(), self.get_end())))
        }
        if args.len() < arg_names.len() {
            return Err(Box::new(RuntimeError::new("Too marny arguments".to_string(), Some(func_ctx.clone()), self.get_start(), self.get_end())))
        }

        let mut scope = func_ctx.as_ref().scope.borrow_mut();

        for arg in 0..arg_names.len() {
            match scope.insert(&arg_names[arg],args[arg].clone(),true,Some(func_ctx.clone())) {
                IResult::Ok(_) => {}
                IResult::Inter(_) => {}
                IResult::Err(err) => return Err(err)
            }
        }

        Ok(())

    }
}