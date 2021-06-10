pub enum IResult<T,E,I> {
    Ok(T),
    Err(E),
    Inter(I)
}

impl<T, E: std::fmt::Debug, I> IResult<T, E, I> {
    pub fn expect(self, msg: &str) -> bool {
        match self {
            IResult::Ok(_) => true,
            IResult::Inter(_) => true,
            IResult::Err(e) => panic!("{}: {:?}", msg, e)
        }
    }
}
