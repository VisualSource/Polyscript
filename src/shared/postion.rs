#[derive(Debug, Clone, Copy)]
pub struct Postion {
    pub idx: isize,
    pub ln: isize,
    pub col: isize
}

impl Default for Postion {
    fn default() -> Postion {
        Postion {
            idx: -1,
            ln: 0,
            col: -1
        }
    }
}

impl Postion {
    pub fn new(idx: Option<isize>, ln: Option<isize>, col: Option<isize>) -> Self {
        Postion {
            idx: idx.unwrap_or(-1),
            ln: ln.unwrap_or(0),
            col: col.unwrap_or(-1)
        }
    }
    pub fn advance(&mut self, current_char: char){
       self.idx += 1;
       self.col += 1;
       if current_char == '\n' {
           self.ln += 1;
           self.col = 0;
       }
    }
}