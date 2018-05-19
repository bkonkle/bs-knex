# bs-knex

[![version](https://img.shields.io/npm/v/bs-knex.svg)](http://npm.im/bs-knex)
[![downloads](https://img.shields.io/npm/dt/bs-knex.svg)](http://npm.im/bs-knex)
[![license](https://img.shields.io/npm/l/express.svg)](LICENSE)

BuckleScript utilities for working with the Node [knex](http://knexjs.org/) library.

## Getting Started

To start working with Knex, first define a config:

```reason
let (to_opt, getWithDefault) = (Js.Nullable.to_opt, Js.Option.getWithDefault);

let connection =
  KnexConfig.Connection.make(
    ~user=Config.Database.username,
    ~password=Config.Database.password,
    ~host=Config.Database.hostname,
    ~port=Config.Database.port,
    ~database=Config.Database.name,
    ()
  );

let pool =
  KnexConfig.Pool.make(
    ~min=Config.Database.poolMin,
    ~max=Config.Database.poolMax,
    ~idleTimeoutMillis=Config.Database.poolIdle,
    ()
  );

let config =
  KnexConfig.make(~client="pg", ~connection, ~pool, ~acquireConnectionTimeout=2000, ());
```

Then you can initialize a client:

```reason
let knex = Knex.make(config);
```

You can now try a raw query to verify the connection:

```reason
knex |> Knex.raw("select now()")
```

## Querying

Use the query builder to structure your request for the database:

```reason
Knex.(
  knex
  |> fromTable("users")
  |> where({"id": id})
  |> update({"first_name": firstName})
)
```

When you're ready to wait for results, call `toPromise`:

```reason
|> then_(
  (results) =>
    switch results {
    /* No user found, so resolve with None to signal onboarding */
    | [||] => resolve(None)
    | users => resolve(Some(users[0]))
    }
)
```

Handle empty results with the `rejectIfAny` handler:

```reason
|> then_(rejectIfEmpty(~error="Unable to update User with id: " ++ id))
```

Handle specific unique violations with the `handleUniqueError` utility:

```reason
|> KnexUtils.handleUniqueError(
  ~name="users_email_unique",
  ~message="That email address is already in use."
)
|> KnexUtils.handleUniqueError(
  ~name="users_user_name_unique",
  ~message="That user name is already in use."
)
```

Finish off your operation by handling any remaining generic database errors with `KnexUtils`:

```reason
|> KnexUtils.handleDbErrors
```

This handles a some common database error cases, which will hopefully grow over time as the library becomes more mature.

## License

[BSD 2-Clause](LICENSE)
