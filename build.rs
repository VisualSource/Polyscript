extern crate embed_resource;

use std::env;
use std::fs;
use std::path::Path;

fn main() {
    embed_resource::compile("resources/manifest.rc");
    embed_resource::compile("resources/icon.rc");

    let version_major = env!("CARGO_PKG_VERSION_MAJOR");
    let version_minor = env!("CARGO_PKG_VERSION_MINOR");
    let version_patch = env!("CARGO_PKG_VERSION_PATCH");
    //let author = env!("CARGO_PKG_AUTHORS");
    let description = env!("CARGO_PKG_DESCRIPTION");
    let debug = env::var_os("PROFILE").unwrap().to_str().unwrap() == "debug";
    let exe_bin = env::var_os("CARGO_PKG_NAME").unwrap().to_string_lossy().to_string();
    let dest_path = Path::new("resources").join("Version.rc");

fs::write(&dest_path, format!("
VS_VERSION_INFO VERSIONINFO
    FILEVERSION 0,{major},{minor},{patch}
    PRODUCTVERSION 0,{major},{minor},{patch}
    FILEFLAGSMASK 0x3fL
    FILEFLAGS 0x{debug}L
    FILEOS 0x40004L
    FILETYPE 0x1L
    FILESUBTYPE 0x0L
    BEGIN
        BLOCK \"StringFileInfo\"
        BEGIN
            BLOCK \"040904b0\"
            BEGIN
                VALUE \"CompanyName\", \"VisualSource\"
                VALUE \"LegalCopyright\", \"Copyright (C) 2021\"
                VALUE \"FileDescription\", \"{description}\"
                VALUE \"FileVersion\", \"{version}\"
                VALUE \"InternalName\", \"{bin}.exe\"
                VALUE \"OriginalFilename\", \"{bin}.exe\"
                VALUE \"ProductName\", \"Polyscript\"
                VALUE \"ProductVersion\", \"{version}\"
            END
        END
        BLOCK \"VarFileInfo\"
        BEGIN
            VALUE \"Translation\", 0x409, 1200
        END
    END",
   major=version_major,
   minor=version_minor,
   patch=version_patch,
   debug= if debug { "1" } else { "0" },
   description=description,
   version=format!("{}.{}.{}.{}",0,version_major,version_minor,version_patch),
   bin=exe_bin,
)).unwrap();
    
    embed_resource::compile("resources/Version.rc");
}