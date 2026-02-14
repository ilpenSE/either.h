# Result<T, E> and Option<T> in C

I implemented `Result<T, E>` and `Option<T>` from Rust to C.

Also this library has its own custom `Error` struct and `ErrorCode` enum. I'm using these in Option.

Option is simply `Result<T, Error>`
