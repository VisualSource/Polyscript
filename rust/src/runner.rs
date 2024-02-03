use crate::interpreter::{Interperter, Ctx, DataType};
use crate::parser::Parser;
use crate::lexer::Lexer;
use std::path::PathBuf;


pub async fn parse(input: String, file: PathBuf, ctx: Ctx) -> anyhow::Result<DataType> {
    let lexer = match Lexer::parse(input, file).await {
        Ok(value) => value,
        Err(err) => {
            error!("{}",err);
            bail!(err);
        }
    };

    let parser = match Parser::parse(&lexer) {
        Ok(value) => value,
        Err(err) => {
            error!("{}",err);
            bail!(err);
        }
    };

    Interperter::run(parser, ctx).await
}