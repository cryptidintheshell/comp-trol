## About:
This project is a remote host-client computer management tool

## Tech stack:
1. wxWidgets
2. Sockets

## Coding standard:
1. use `snake_case` when naming variables
2. when naming a `wxWidgets` object, write first its synonym, followed by the name of the variable base on its function (like `wxButton* button_confirm` to `wxButton*  btn_confirm`)

## Architecture rules:
1. when creating a `class`, seperate the class method definition from its class method declaration 
2. `.cpp` files will be placed in the `src` folder
3. `.h` (class) files will be placed in the `headers` folder