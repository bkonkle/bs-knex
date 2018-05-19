open Jest;

let connection = KnexConfig.Connection.make(~filename=":memory:", ());

let config = KnexConfig.make(~client="sqlite3", ~connection, ());

describe(
  "Knex",
  () =>
    describe(
      "queryBuilder",
      () => {
        open ExpectJs;
        let knex = Knex.make(config);
        test(
          "returns the correct sql",
          () => {
            let sql =
              knex |> Knex.fromTable("users") |> Knex.where({"id": "awesome"}) |> Knex.toSQL();
            expect(sql##sql) |> toEqual("select * from `users` where `id` = ?")
          }
        );
        test(
          "returns the correct bindings",
          () => {
            let sql =
              knex |> Knex.fromTable("users") |> Knex.where({"id": "awesome"}) |> Knex.toSQL();
            expect(sql##bindings) |> toEqual([|"awesome"|])
          }
        )
      }
    )
);
