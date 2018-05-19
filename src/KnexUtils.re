open Js.Promise;

let getEnvVar = (key, fallback) =>
  Js.Option.getWithDefault(fallback, Js.Dict.get(Node.Process.process##env, key));

let debug = Debug.make(getEnvVar("KNEX_DEBUG_PREFIX", "bs-knex"), "KnexUtils");

let debugExn = Debug.make(getEnvVar("KNEX_DEBUG_PREFIX", "bs-knex"), "KnexUtils:exn");

[@bs.module] external parseDbUrl : string => KnexConfig.Db.t = "parse-database-url";

[@bs.new] external makeError : string => exn = "Error";

/* Database Exceptions */
/** Retrieve the "code" from a database exception */ [@bs.get]
external exnCode : Js.Promise.error => Js.Nullable.t(string) =
  "code";

/** Retrieve the "constraint" from a database exception */ [@bs.get]
external exnConstraint : Js.Promise.error => Js.Nullable.t(string) =
  "constraint";

/** Retrieve the "routine" from a database exception */ [@bs.get]
external exnRoutine : Js.Promise.error => Js.Nullable.t(string) =
  "routine";

let invalidTextRepresentation = "22P02";

let uniqueViolation = "23505";

/**
 * Safely converts an object to json by stringifying it and parsing the results.
 */
let objToJson = (obj: Js.t('a)) : Js.Json.t =>
  Js.Option.(
    Js.Json.(
      stringifyAny(obj)
      |> andThen(
           [@bs]
           (
             (str) =>
               try (str |> parseExn |> some) {
               | _exn => None
               }
           )
         )
      |> getWithDefault(null)
    )
  );


/***
 * Pass-through error handlers - handles a specific error, letting all the rest flow through to the
 * next handler.
 */
/**
 * Catch unique errors and pass the exception to a handler function that should return a promise
 */
let catchUniqueError = (~name: string, ~handle, promise) =>
  Js.Nullable.(
    promise
    |> catch(
         (exn) => {
           let continue = reject(Debug.toExn(exn));
           let codeOpt = exn |> exnCode |> toOption;
           switch codeOpt {
           | Some(code) =>
             if (code === uniqueViolation) {
               let constraintOpt = exn |> exnConstraint |> toOption;
               switch constraintOpt {
               | Some(constraintName) =>
                 if (constraintName === name) {
                   handle(exn)
                 } else {
                   continue
                 }
               | None => continue
               }
             } else {
               continue
             }
           | None => continue
           }
         }
       )
  );

/**
 * Handles errors for the specified unique constraint
 */
let handleUniqueError = (~name: string, ~message: string) =>
  catchUniqueError(~name, ~handle=(_exn) => reject(makeError(message)));


/***
 * Terminator error handlers - handles all remaining unhandled DB errors
 */
/** Handle generic database errors */
let handleDbErrors = (promise) =>
  Js.Nullable.(
    promise
    |> catch(
         (exn) => {
           let codeOpt = exn |> exnCode |> toOption;
           switch codeOpt {
           | Some(code) =>
             if (code === uniqueViolation) {
               reject(makeError("A unique constraint was violated."))
             } else if (code === invalidTextRepresentation) {
               let routineOpt = exn |> exnRoutine |> toOption;
               switch routineOpt {
               | Some(routine) =>
                 if (routine === "string_to_uuid") {
                   reject(makeError("The database received an invalid format for a UUID"))
                 } else {
                   reject(makeError("The database received an invalid text representation."))
                 }
               | None => reject(makeError("The database received an invalid text representation."))
               }
             } else {
               debug("Unhandled error:");
               debugExn(exn);
               reject(makeError("A database error occurred."))
             }
           /* There's no "code" property on the error, so this isn't a Knex DB error */
           | None => reject(Debug.toExn(exn))
           }
         }
       )
  );

let decodeResults = (~decoder: Js.Json.t => 'a, results: array('row)) =>
  Js.Array.(results |> map(objToJson) |> map(decoder) |> resolve);

let rejectIfEmpty = (~decoder, ~error, results) =>
  switch results {
  | [||] => reject(makeError(error))
  | results => decodeResults(~decoder, results)
  };

let pickFirst = (results) => resolve(results[0]);
