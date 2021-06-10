extern crate reqwest;
use super::traits::BuiltInFunc;
use super::Varible;
use crate::errors::{error::Error, RuntimeError};
use crate::shared::{context::Context, result::IResult, postion::Postion, scope::Scope };
use std::rc::Rc;
use std::fs::read_to_string;
use std::path::Path;


#[derive(Clone, Debug)]
struct Print {}
impl BuiltInFunc for Print {
    fn exceute(&self, _args: Vec<Varible>, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {

        match ctx.as_ref().scope.borrow().get(&"__input",Some(ctx.clone())) {
            IResult::Ok(value) => {
                print!("{}",value);
                IResult::Ok(Varible::Null)
            }
            IResult::Err(error) => IResult::Err(error),
            IResult::Inter(_) => {
                IResult::Err(Box::new(RuntimeError::new("Invaild value".to_string(), Some(ctx.clone()), Postion::default(), Postion::default())))
            }
        }

    }
}

#[derive(Clone, Debug)]
struct PrintLn {}
impl BuiltInFunc for PrintLn {
    fn exceute(&self, _args: Vec<Varible>, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {

        match ctx.as_ref().scope.borrow().get(&"__input",Some(ctx.clone())) {
            IResult::Ok(value) => {
                println!("{}",value);
                IResult::Ok(Varible::Null)
            }
            IResult::Err(error) => IResult::Err(error),
            IResult::Inter(_) => {
                IResult::Err(Box::new(RuntimeError::new("Invaild value".to_string(), Some(ctx.clone()), Postion::default(), Postion::default())))
            }
        }

    }
}

#[derive(Clone, Debug)]
struct Len {}
impl BuiltInFunc for Len {
    fn exceute(&self, _args: Vec<Varible>, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {

        match ctx.as_ref().scope.borrow().get(&"__input",Some(ctx.clone())) {
            IResult::Ok(value) => {
                match value {
                    Varible::String(value) => {
                        IResult::Inter(Varible::new_integer(value.len() as isize))
                    }
                    Varible::List(value) => { 
                        IResult::Inter(Varible::new_integer(value.len() as isize))
                    }
                    _ => {
                        IResult::Err(Box::new(RuntimeError::new("Invaild value".to_string(), Some(ctx.clone()), Postion::default(), Postion::default())))
                    }
                }
            }
            IResult::Err(error) => IResult::Err(error),
            IResult::Inter(_) => {
                IResult::Err(Box::new(RuntimeError::new("Invaild value".to_string(), Some(ctx.clone()), Postion::default(), Postion::default())))
            }
        }

    }
}

#[derive(Clone, Debug)]
struct IsFunction {}
impl BuiltInFunc for IsFunction  {
    fn exceute(&self, _args: Vec<Varible>, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {
        match ctx.as_ref().scope.borrow().get(&"__input",Some(ctx.clone())) {
            IResult::Ok(value) => {
                match value {
                    Varible::Function(_) => {
                        IResult::Inter(Varible::new_bool(true))
                    }
                    _ => {
                        IResult::Inter(Varible::new_bool(false))
                    } 
                }
            }
            IResult::Err(error) => IResult::Err(error),
            IResult::Inter(_) => {
                IResult::Err(Box::new(RuntimeError::new("Invaild value".to_string(), Some(ctx.clone()), Postion::default(), Postion::default())))
            }
        }

    }
}
#[derive(Clone, Debug)]
struct IsString {}
impl BuiltInFunc for IsString {
    fn exceute(&self, _args: Vec<Varible>, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {
        match ctx.as_ref().scope.borrow().get(&"__input",Some(ctx.clone())) {
            IResult::Ok(value) => {
                match value {
                    Varible::String(_) => {
                        IResult::Inter(Varible::new_bool(true))
                    }
                    _ => {
                        IResult::Inter(Varible::new_bool(false))
                    } 
                }
            }
            IResult::Err(error) => IResult::Err(error),
            IResult::Inter(_) => {
                IResult::Err(Box::new(RuntimeError::new("Invaild value".to_string(), Some(ctx.clone()), Postion::default(), Postion::default())))
            }
        }

    }
}

#[derive(Clone, Debug)]
struct IsInteger {}
impl BuiltInFunc for IsInteger  {
    fn exceute(&self, _args: Vec<Varible>, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {
        match ctx.as_ref().scope.borrow().get(&"__input",Some(ctx.clone())) {
            IResult::Ok(value) => {
                match value {
                    Varible::Integer(_) => {
                        IResult::Inter(Varible::new_bool(true))
                    }
                    _ => {
                        IResult::Inter(Varible::new_bool(false))
                    } 
                }
            }
            IResult::Err(error) => IResult::Err(error),
            IResult::Inter(_) => {
                IResult::Err(Box::new(RuntimeError::new("Invaild value".to_string(), Some(ctx.clone()), Postion::default(), Postion::default())))
            }
        }

    }
}

#[derive(Clone, Debug)]
struct IsFloat {}
impl BuiltInFunc for IsFloat  {
    fn exceute(&self, _args: Vec<Varible>, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {
        match ctx.as_ref().scope.borrow().get(&"__input",Some(ctx.clone())) {
            IResult::Ok(value) => {
                match value {
                    Varible::Float(_) => {
                        IResult::Inter(Varible::new_bool(true))
                    }
                    _ => {
                        IResult::Inter(Varible::new_bool(false))
                    } 
                }
            }
            IResult::Err(error) => IResult::Err(error),
            IResult::Inter(_) => {
                IResult::Err(Box::new(RuntimeError::new("Invaild value".to_string(), Some(ctx.clone()), Postion::default(), Postion::default())))
            }
        }

    }
}

#[derive(Clone, Debug)]
struct IsEnum {}
impl BuiltInFunc for IsEnum  {
    fn exceute(&self, _args: Vec<Varible>, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {
        match ctx.as_ref().scope.borrow().get(&"__input",Some(ctx.clone())) {
            IResult::Ok(value) => {
                match value {
                    Varible::Enum(_) => {
                        IResult::Inter(Varible::new_bool(true))
                    }
                    Varible::EnumValue(_) => {
                        IResult::Inter(Varible::new_bool(true))
                    }
                    _ => {
                        IResult::Inter(Varible::new_bool(false))
                    } 
                }
            }
            IResult::Err(error) => IResult::Err(error),
            IResult::Inter(_) => {
                IResult::Err(Box::new(RuntimeError::new("Invaild value".to_string(), Some(ctx.clone()), Postion::default(), Postion::default())))
            }
        }

    }
}

#[derive(Clone, Debug)]
struct IsNull {}
impl BuiltInFunc for IsNull {
    fn exceute(&self, _args: Vec<Varible>, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {
        match ctx.as_ref().scope.borrow().get(&"__input",Some(ctx.clone())) {
            IResult::Ok(value) => {
                match value {
                    Varible::Null => {
                        IResult::Inter(Varible::new_bool(true))
                    }
                    _ => {
                        IResult::Inter(Varible::new_bool(false))
                    } 
                }
            }
            IResult::Err(error) => IResult::Err(error),
            IResult::Inter(_) => {
                IResult::Err(Box::new(RuntimeError::new("Invaild value".to_string(), Some(ctx.clone()), Postion::default(), Postion::default())))
            }
        }

    }
}

#[derive(Clone, Debug)]
struct ReadFile {}
impl BuiltInFunc for ReadFile {
    fn exceute(&self, _args: Vec<Varible>, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {
        match ctx.as_ref().scope.borrow().get(&"__input",Some(ctx.clone())) {
            IResult::Ok(value) => {
                match value {
                    Varible::String(input_path) => {
                        let path = Path::new(&input_path);

                        if !path.is_file() {
                            return IResult::Err(Box::new(RuntimeError::new("path is not a file".to_string(), Some(ctx.clone()), Postion::default(), Postion::default())))
                        }

                        match read_to_string(path) {
                            Ok(content) => {
                                IResult::Inter(Varible::String(content))
                            }
                            Err(_) => {
                                IResult::Err(Box::new(RuntimeError::new("failed to read file to string".to_string(), Some(ctx.clone()), Postion::default(), Postion::default())))
                            }
                        }
                    }
                    _ => {
                        IResult::Err(Box::new(RuntimeError::new("Invaild value".to_string(), Some(ctx.clone()), Postion::default(), Postion::default())))
                    } 
                }
            }
            IResult::Err(error) => IResult::Err(error),
            IResult::Inter(_) => {
                IResult::Err(Box::new(RuntimeError::new("Invaild value".to_string(), Some(ctx.clone()), Postion::default(), Postion::default())))
            }
        }

    }
}

#[derive(Clone, Debug)]
struct Fetch {
}
impl Fetch {
    fn new() -> Self {
        Fetch {
           
        }
    }
}
impl BuiltInFunc for Fetch {
    fn exceute(&self, _args: Vec<Varible>, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {
        match ctx.as_ref().scope.borrow().get(&"__input",Some(ctx.clone())) {
            IResult::Ok(value) => {
                match value {
                    Varible::String(url) => {
                       match reqwest::blocking::get(url) {
                           Ok(value) => {
                               IResult::Inter(Varible::new_object(&vec![
                                   (String::from("status"), Varible::new_integer(value.status().as_u16() as isize)),
                                   (String::from("text"), Varible::String(value.text().unwrap()))
                               ]))
                           }
                           Err(_err) => {
                            IResult::Err(Box::new(RuntimeError::new("request error".to_string(), Some(ctx.clone()), Postion::default(), Postion::default())))
                           }
                       }
                    }
                    _ => {
                        IResult::Err(Box::new(RuntimeError::new("Invaild value".to_string(), Some(ctx.clone()), Postion::default(), Postion::default())))
                    } 
                }
            }
            IResult::Err(error) => IResult::Err(error),
            IResult::Inter(_) => {
                IResult::Err(Box::new(RuntimeError::new("Invaild value".to_string(), Some(ctx.clone()), Postion::default(), Postion::default())))
            }
        }

    }
}

pub fn register_buitin(scope: &mut Scope) -> std::io::Result<()> {

    scope.insert(&"print", Varible::new_builtin("print", vec!["__input".to_string()], Box::new(Print {})), false, None).expect("builtin failed");
    scope.insert(&"println", Varible::new_builtin("println", vec!["__input".to_string()], Box::new(PrintLn {})), false, None).expect("builtin failed");
    scope.insert(&"len", Varible::new_builtin("len", vec!["__input".to_string()], Box::new(Len {})), false, None).expect("builtin failed");
    scope.insert(&"isFunction", Varible::new_builtin("isFunction", vec!["__input".to_string()], Box::new(IsFunction {})), false, None).expect("builtin failed");
    scope.insert(&"isString", Varible::new_builtin("isString", vec!["__input".to_string()], Box::new(IsString {})), false, None).expect("builtin failed");
    scope.insert(&"isInteger", Varible::new_builtin("isInteger", vec!["__input".to_string()], Box::new(IsInteger {})), false, None).expect("builtin failed");
    scope.insert(&"isFloat", Varible::new_builtin("isFloat", vec!["__input".to_string()], Box::new(IsFloat {})), false, None).expect("builtin failed");
    scope.insert(&"isEnum", Varible::new_builtin("isEnum", vec!["__input".to_string()], Box::new(IsEnum {})), false, None).expect("builtin failed");
    scope.insert(&"isNull", Varible::new_builtin("isNull", vec!["__input".to_string()], Box::new(IsNull {})), false, None).expect("builtin failed");
    scope.insert(&"readFile", Varible::new_builtin("readFile", vec!["__input".to_string()], Box::new(ReadFile {})), false, None).expect("builtin failed");

    scope.insert(&"fetch", Varible::new_builtin("fetch", vec!["__input".to_string()], Box::new(Fetch::new())), false, None).expect("builtin failed");


    Ok(())
}