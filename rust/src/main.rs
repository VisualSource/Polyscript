#[macro_use]
extern crate log;
#[macro_use]
extern crate anyhow;

extern crate simple_logger;

mod errors;
mod shared;
mod lexer;
mod parser;
mod interpreter;
mod standard_lib;
mod runner;
use std::env;
use std::sync::{Arc,Mutex};
use interpreter::Context;
use std::path::PathBuf;
use std::io::{ stdin, stdout, Write };
use tokio::fs::read_to_string;
use simple_logger::SimpleLogger;

use crate::runner::parse;

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    SimpleLogger::new().init().unwrap();
    let args: Vec<String> = env::args().collect();
    let file = if let Some(value) = args.get(1) {
        PathBuf::from(value)
    } else {
        match env::current_dir() {
            Ok(value) => value,
            Err(_err) => PathBuf::from("/")
        }
    };

    let ctx = Arc::new(
        Mutex::new(
            Context::default(file.clone())
        ));

    if file.is_file() {
        match read_to_string(file.clone()).await {
            Ok(value) => {
                match runner::parse(value,file,ctx).await {
                    Ok(_) => return Ok(()),
                    Err(err) => return Err(err)
                }
            }
            Err(err) => bail!(err)
        }
    }

    let mut buffer = String::default();

    println!("Vip 0.0.1: type '.exit' to close");
    loop {
        print!("> ");
        stdout().flush().expect("Faild to flush");
        match stdin().read_line(&mut buffer) {
            Ok(_) => {
                if buffer.trim() == ".exit" { break; }
                match parse(buffer.clone(),file.clone(),ctx.clone()).await {
                    Ok(value) => println!("{}",value),
                    Err(err) => eprintln!("{}",err)
                }
                buffer.clear();
            }
            Err(err) => {
                error!("{}",err);
            }
        }
    }

    Ok(())
}
