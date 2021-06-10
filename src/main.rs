extern crate colored;
mod errors;
mod shared;
mod types;
mod lexer;
mod parser;
mod interpreter;
mod config;

use lexer::Lexer;
use parser::Parser;
use interpreter::Interpreter;
use shared::{context::Context, scope::Scope, result::IResult};
use types::{builtins::register_buitin, variabledef::register_variables};
use types::Varible;
use std::rc::Rc;
use std::cell::RefCell;
use std::fs::read_to_string;
use std::path::Path;
use std::io::{Result, stdin, stdout, Write};

/// handles the parsing and error output when running from the command line.
fn run(text: String, file_name: String, globalscope: Rc<RefCell<Scope>>, config: &config::Config) -> Result<()> {

    let mut lex = Lexer::new(text, file_name);

   let result = lex.make_tokens();
   match result {
        Ok(e) => {
           // println!("{:#?}",e);
            let mut parser = Parser::new(e.0);
            let node = parser.parse();
            match node {
                Err(err) => {
                    println!("{}",err.to_string());
                }
                Ok(value) => {
                    let global_context = Rc::new(Context::new("<program>", e.1.as_str(), globalscope, None, None));
                    let interpreter = Interpreter::new();

                    match interpreter.visit(value, global_context) {
                        IResult::Err(error) => {
                            eprintln!("{}",error.to_string());
                        }
                        IResult::Inter(_) => {}
                        IResult::Ok(par) => {
                            match par {
                                Varible::List(list) => {
                                    if config.from_file() {
                                        println!("{}",list[0])
                                    }
                                }
                                a => println!("{}",a)
                            }
                        }
                    }
                }
            }
            
        }
        Err(e) =>{
            println!("{}",e.to_string());
        }
    }



    Ok(())
}

/// sets up the global scope variables and functions
fn init() -> Scope {

    let mut scope = Scope::new();

    register_variables(&mut scope).expect("Built init error");
    register_buitin(&mut scope).expect("Built init error");

    scope
}
/// handles running from the command line
fn command_line(globalscope: Rc<RefCell<Scope>>, config: config::Config) -> Result<()>{

    let mut cout = stdout();
    let cin =  stdin();

    let mut input = String::new();
    let file_name = String::from("<stdin>");

    println!("Polyscript V{} | to exit use 'exit()' ", config.version);
    loop {
        cout.write_all(b"> ")?;
        cout.flush()?;
        match cin.read_line(&mut input) {
            Ok(_) => {
                if input.contains("exit()") {
                    break;
                }
                run(input.clone(),file_name.clone(), globalscope.clone(), &config)?;
            }
            Err(error) => println!("error: {}", error),
        }
        input.clear();
    }
  
    Ok(())
}

fn main() -> Result<()> {

    let globalscope = Rc::new(RefCell::new(init()));
    
    let config = config::Config::new();

    if config.file == "<stdin>" {
        command_line(globalscope, config)
    }else {
        let path = Path::new(&config.file);

        if !path.is_file() {
            panic!("Expectd file not a dir");
        }

        match read_to_string(path) {
            Ok(content) => {
                run(content, path.file_name().expect("Expected File").to_str().expect("Expected String").to_string(), globalscope, &config)
            }
            Err(err) => Err(err)
        }
    }

}
