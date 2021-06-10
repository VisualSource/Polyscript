extern crate clap;
use clap::{Arg, App};

pub struct Config {
   pub file: String,
   pub version: String,
}

impl Config {
    pub fn new() -> Self {
        let version = option_env!("CARGO_PKG_VERSION").unwrap();
        let author = option_env!("CARGO_PKG_AUTHORS").unwrap();
        let matches = App::new("Polyscript")
        .version(version)
        .author(author)
        .arg(Arg::with_name("run").value_name("FILE").takes_value(true)).get_matches();

    
        Self {
            file: matches.value_of("run").unwrap_or("<stdin>").to_string(),
            version: version.to_string(),
        }
    }

    pub fn from_file(&self) -> bool {
        self.file == "<stdin>"
    }
}


