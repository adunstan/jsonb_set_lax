


\pset null NULL

-- pass though non nulls to jsonb_set
select jsonb_set_lax('{"a":1,"b":2}','{b}','5') ;
select jsonb_set_lax('{"a":1,"b":2}','{d}','6', true) ;

-- using the default treatment
select jsonb_set_lax('{"a":1,"b":2}','{b}',null);
select jsonb_set_lax('{"a":1,"b":2}','{d}',null,true);

-- errors
select jsonb_set_lax('{"a":1,"b":2}', '{b}', null, true, null);
select jsonb_set_lax('{"a":1,"b":2}', '{b}', null, true, 'no_such_treatment');

-- explicit treatments
select jsonb_set_lax('{"a":1,"b":2}', '{b}', null, null_value_treatment := 'raise_exception') as raise_exception;
select jsonb_set_lax('{"a":1,"b":2}', '{b}', null, null_value_treatment := 'return_target') as return_target;
select jsonb_set_lax('{"a":1,"b":2}', '{b}', null, null_value_treatment := 'delete_key') as delete_key;
select jsonb_set_lax('{"a":1,"b":2}', '{b}', null, null_value_treatment := 'use_json_null') as use_json_null;
