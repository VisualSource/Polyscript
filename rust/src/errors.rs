use thiserror::Error;
use std::path::PathBuf;
use crate::shared::Position;

#[derive(Error,Debug)]
pub enum LexerError {
    #[error("Illegal Char '{illegal:?}' at Line {pos} in {file:?}")]
    IllegalCharError {
        pos: Position,
        illegal: String,
        file: PathBuf
    },
    #[error("Syntax Error at Line {pos} in {file:?}")]
    SyntaxError {
        pos: Position,
        file: PathBuf
    }
} 

#[derive(Error,Debug)]
pub enum ParserError {
    #[error("{reason} at {pos}")]
    SyntaxError {
        pos: Position,
        reason: &'static str
    }
}


#[derive(Error,Debug)]
pub enum RuntimeError {
    #[error("{module} - {error}")]
    StdError {
        module: &'static str,
        error: std::io::Error
    },
    #[error("{reason} at {start}-{end} in {ctx:?}")]
    InterptError {
        start: Position,
        end: Position,
        reason: &'static str,
        ctx: &'static str
    },
    #[error("Attempted operation '{op}' on a non number type")]
    OperationError {
        op: &'static str,
    },
    #[error("ReferenceError: {reason}")]
    ReferenceError {
        reason: String
    },
    #[error("TypeError: {reason}")]
    TypeError {
        reason: String
    },
    #[error("InternalError: {reason}")]
    InternalError {
        reason: &'static str
    },
    #[error("RangeError: {reason}")]
    RangeError {
        reason: &'static str
    },
    #[error("SyntaxError: {reason}")]
    SyntaxError {
        reason: String
    },
    #[error("Error: {0}")]
    Error(anyhow::Error)
}