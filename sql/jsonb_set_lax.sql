-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION jsonb_set_lax" to load this file. \quit

CREATE FUNCTION
  jsonb_set_lax(jsonb_in jsonb, path text[] , replacement jsonb,
            create_if_missing boolean DEFAULT true,
            null_value_treatment text DEFAULT 'use_json_null')
RETURNS jsonb
LANGUAGE C
CALLED ON NULL INPUT IMMUTABLE PARALLEL SAFE
AS 'MODULE_PATHNAME';
