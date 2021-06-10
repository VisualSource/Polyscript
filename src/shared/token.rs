use super::postion::Postion;
use super::token_type::TokenType;


#[derive(Debug, Clone)]
pub struct Token {
    pub token: TokenType,
    pub value: Option<String>,
    pub start: Postion,
    pub end: Postion
}
impl Token {
    pub fn new(token: TokenType, value: Option<String>, start: Postion, end: Option<Postion>) -> Self {
        Token {
            token,
            value,
            start: start.clone(),
            end: end.unwrap_or(start)
        }
    }
    pub fn is(&self, token: &TokenType) -> bool {
        token == &self.token
    }
    pub fn matches<T>(&self, token: &TokenType, value: &T) -> bool where T: ToString {
        self.is(token) && match &self.value {
            Some(x) => x == &value.to_string(),
            None => false,
        }
    }
    pub fn is_keyword<T>(&self, value: &T) -> bool where T: ToString {
        self.matches(&TokenType::KEYWORD, &value.to_string())
    }
    pub fn to_string(&self) -> String {
        if self.value.is_some() {
            format!("{}:{}",self.token.to_string(),self.value.as_ref().unwrap()).to_string()
        } else {
            format!("{}",self.token.to_string()).to_string()
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn create_token() {
        let token = Token::new(TokenType::INT, None, Postion::default(), None);
        assert_eq!(token.value,None);
        assert_eq!(token.token,TokenType::INT);
    }
    #[test]
    fn is_token_test(){
        let token = Token::new(TokenType::INT, None, Postion::default(), None);
        assert_eq!(token.is(&TokenType::INT),true);
    }
    #[test]
    fn is_token_matches_test(){
        let token = Token::new(TokenType::IDENTIFIER, Some("var".to_string()), Postion::default(), None);
        assert_eq!(token.matches(&TokenType::IDENTIFIER, &"var".to_string()),true);
        assert_eq!(token.matches(&TokenType::IDENTIFIER, &"doc".to_string()),false);
    }
    #[test]
    fn is_token_keyword(){
        let token = Token::new(TokenType::KEYWORD, Some("fn".to_string()), Postion::default(), None);
        assert_eq!(token.is_keyword(&"fn".to_string()),true);
        assert_eq!(token.is_keyword(&"class".to_string()),false);
    }
    #[test]
    fn print(){
        let token = Token::new(TokenType::KEYWORD, Some("fn".to_string()), Postion::default(), None);
        println!("{}",token.to_string());
    }
}
