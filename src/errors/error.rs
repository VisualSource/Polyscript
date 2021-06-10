use crate::shared::postion::Postion;

pub trait Error {
    fn get_start(&self) -> Postion;
    fn get_end(&self) -> Postion;
    fn get_details(&self) -> String;
    fn get_name(&self) -> String;
    fn to_string(&self) -> String;
}
impl std::fmt::Debug for dyn Error {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("Error")
        .field("Name", &self.get_name())
        .field("Details", &self.get_details())
        .field("Start Postion", &self.get_start())
        .finish()
    }
}

