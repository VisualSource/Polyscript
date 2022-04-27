use std::collections::HashMap;
use crate::interpreter::{ Functional, VisitResult, DataType, Property };
use crate::errors::RuntimeError;
use async_trait::async_trait;
use std::str::FromStr;

#[derive(Debug,Clone)]
struct Fetch {}

impl Fetch {
    fn new() -> DataType {
        DataType::NativeFunction { 
            name: "fetch".into(), 
            args: vec!["url".into(),"opt".into()], 
            method: Box::new(Self {}) 
        }
    } 
}

#[async_trait]
impl Functional for Fetch {
    async fn execute(&self, args: Vec<crate::interpreter::DataType>) -> crate::interpreter::VisitResult<crate::interpreter::DataType> {
        let url = match args.get(0) {
            Some(arg) => {
                match arg {
                    DataType::STRING { value } => value.to_owned(),
                    _ => return VisitResult::Error(RuntimeError::TypeError { reason: "Expected type of 'string'".into() })
                }
            }
            None => return VisitResult::Error(RuntimeError::ReferenceError { reason: "Missing url argument".into() })
        };
        let opt = match args.get(1) {
            Some(arg) => {
                match arg {
                    DataType::OBJECT { value } => {
                        Some(value.clone())
                    }
                    _ => return VisitResult::Error(RuntimeError::TypeError { reason: "Expected arg type of 'object'".into() })
                }
            }
            None => None
        };

        let method = if let Some(opts) = opt {
            if let Some(moth) = opts.get("method") {
                match moth {
                    DataType::STRING { value } => {
                        match reqwest::Method::from_str(value.to_uppercase().as_str()) {
                            Ok(value) => value,
                            Err(err) => {
                                error!("{}",err);
                                return VisitResult::Error(RuntimeError::InternalError { reason: "Invaild method".into() });
                            }
                        }
                    },
                    _ => return VisitResult::Error(RuntimeError::TypeError { reason: "Expected arg type of 'string'".into() })
                }
            } else {
                reqwest::Method::GET
            }
        } else {
            reqwest::Method::GET
        };

        let client = reqwest::Client::new();

     
        let res = match client.request(method, url).send().await {
            Ok(value) => value,
            Err(err) => {
                error!("{}",err);
                return VisitResult::Error(RuntimeError::InternalError { reason: "Error in making request".into() });
            }
        };

        info!("{:#?}",res);

        VisitResult::Ok(DataType::NULL)
    }
}


pub fn insert_http(data: &mut HashMap<String,Property>) {
    data.insert("fetch".into(), Property { 
        value: Fetch::new(),
        writable: false
     });
}