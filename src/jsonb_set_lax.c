

#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "utils/jsonb.h"

PG_MODULE_MAGIC;

/* functions we refer to */
extern Datum jsonb_set(PG_FUNCTION_ARGS);
extern Datum jsonb_delete_path(PG_FUNCTION_ARGS);


/* our function */
extern Datum jsonb_set_lax(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(jsonb_set_lax);

/*
 * SQL function jsonb_set_lax(jsonb, text[], jsonb, boolean, text)
 */
Datum
jsonb_set_lax(PG_FUNCTION_ARGS)
{
	/* Jsonb	   *in = PG_GETARG_JSONB_P(0); */
	/* ArrayType  *path = PG_GETARG_ARRAYTYPE_P(1); */
	/* Jsonb	  *newval = PG_GETARG_JSONB_P(2); */
	/* bool		create = PG_GETARG_BOOL(3); */
	text       *handle_null;
	char       *handle_val;

	if (PG_ARGISNULL(0) || PG_ARGISNULL(1) || PG_ARGISNULL(3))
		PG_RETURN_NULL();

	/* could happen if they pass in an explicit NULL */
	if (PG_ARGISNULL(4))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("need delete_key, return_target, use_json_null, or raise_exception")));

	/* if the new value isn't an SQL NULL just call jsonb_set */
	if (! PG_ARGISNULL(2))
		return jsonb_set(fcinfo);

	handle_null = PG_GETARG_TEXT_P(4);
	handle_val = text_to_cstring(handle_null);

	if (strcmp(handle_val,"raise_exception") == 0)
	{
		ereport(ERROR,
				(errcode(ERRCODE_NULL_VALUE_NOT_ALLOWED),
				 errmsg("NULL is not allowed"),
				 errdetail("exception raised due to \"null_value_treatment => 'raise_exception'\""),
				 errhint("to avoid, either change the null_value_treatment argument or ensure that an SQL NULL is not used")));
	}
	else if (strcmp(handle_val, "use_json_null") == 0)
	{
		Datum	  newval;

		newval = DirectFunctionCall1(jsonb_in, CStringGetDatum("null"));

		fcinfo->args[2].value = newval;
		fcinfo->args[2].isnull = false;
		return jsonb_set(fcinfo);
	}
	else if (strcmp(handle_val, "delete_key") == 0)
	{
		return jsonb_delete_path(fcinfo);
	}
	else if (strcmp(handle_val, "return_target") == 0)
	{
		Jsonb	   *in = PG_GETARG_JSONB_P(0);
		PG_RETURN_JSONB_P(in);
	}
	else
	{
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("need delete_key, return_target, use_json_null, or raise_exception")));
	}
}


