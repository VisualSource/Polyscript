use crate::shared::scope::Scope;
use super::Varible;
use std::env;

pub fn register_variables(scope: &mut Scope) -> std::io::Result<()> {

    scope.insert(&"null", Varible::Null, false, None).expect("builtin failed");
    scope.insert(&"true", Varible::new_bool(true), false, None).expect("builtin failed");
    scope.insert(&"false", Varible::new_bool(false), false, None).expect("builtin failed");

    
    let mut env_args: Vec<(String,Varible)> = vec![];
    for (key, value) in env::vars_os() {
        env_args.push((key.to_str().expect("Expected String").to_string(),Varible::String(value.to_str().expect("Expected String").to_string())))
    }
    scope.insert(&"env", Varible::new_object(&env_args), true, None).expect("builtin failed");
    scope.insert(&"JSON", Varible::new_object(&vec![("parse".to_string(),Varible::Null),("stringify".to_string(),Varible::Null)]), true, None).expect("builtin failed");

    Ok(())

}