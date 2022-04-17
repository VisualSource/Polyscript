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
use std::env;
use std::sync::{Arc,Mutex};
use interpreter::{Context, Interperter, DataType };
use std::path::PathBuf;
use std::io::{ stdin, stdout, Write };
use tokio::fs::read_to_string;

use simple_logger::SimpleLogger;

async fn run(input: String, file: PathBuf, ctx: Arc<Mutex<Context>>) -> anyhow::Result<DataType> {
    let lexer = match lexer::Lexer::parse(input, file).await {
        Ok(value) => value,
        Err(err) => {
            error!("{}",err);
            bail!(err);
        }
    };

    //debug!("{:?}",lexer);

    let parser = match parser::Parser::parse(&lexer) {
        Ok(value) => value,
        Err(err) => {
            error!("{}",err);
            bail!(err);
        }
    };

    //debug!("{:?}",parser);

    Interperter::run(parser, ctx).await
}

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    SimpleLogger::new().init().unwrap();
    let args: Vec<String> = env::args().collect();
    let file = if let Some(value) = args.get(1) {
        PathBuf::from(value)
    } else {
        PathBuf::from("<INTERNAL>")
    };

    let ctx = Arc::new(
        Mutex::new(
            Context::default(file.clone())
        ));

    if file.is_file() {
        match read_to_string(file.clone()).await {
            Ok(value) => {
                match run(value,file,ctx).await {
                    Ok(_) => return Ok(()),
                    Err(err) => return Err(err)
                }
            }
            Err(err) => bail!(err)
        }
    }

    let mut buffer = String::default();

    println!("Polyscript V2 0.0.1: type '.exit' to close");
    loop {
        print!("> ");
        stdout().flush().expect("Faild to flush");
        match stdin().read_line(&mut buffer) {
            Ok(_) => {
                if buffer.trim() == ".exit" { break; }
                match run(buffer.clone(),file.clone(),ctx.clone()).await {
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
