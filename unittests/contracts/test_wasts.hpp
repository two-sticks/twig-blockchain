#pragma once
#include <eosio/chain/webassembly/common.hpp>

static const char huge_tables_wast[] = R"=====(
(module
 (import "env" "sha256" (func $sha256 (param i32 i32 i32)))
 (table 0 anyfunc)
 (memory $0 32)
 (data (i32.const 4) "hello")
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64)
  (call $sha256
   (i32.const 4)
   (i32.const 5)
   (i32.const 16)
  )
 )
)
)=====";

static const char aligned_ref_wast[] = R"=====(
(module
 (import "env" "sha256" (func $sha256 (param i32 i32 i32)))
 (table 0 anyfunc)
 (memory $0 32)
 (data (i32.const 4) "hello")
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64)
  (call $sha256
   (i32.const 4)
   (i32.const 5)
   (i32.const 16)
  )
 )
)
)=====";

static const char aligned_const_ref_wast[] = R"=====(
(module
 (import "env" "sha256" (func $sha256 (param i32 i32 i32)))
 (import "env" "assert_sha256" (func $assert_sha256 (param i32 i32 i32)))
 (table 0 anyfunc)
 (memory $0 32)
 (data (i32.const 4) "hello")
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64)
  (local $3 i32)
  (call $sha256
   (i32.const 4)
   (i32.const 5)
   (i32.const 16)
  )
  (call $assert_sha256
   (i32.const 4)
   (i32.const 5)
   (i32.const 16)
  )
 )
)
)=====";

static const char misaligned_ref_wast[] = R"=====(
(module
 (import "env" "sha256" (func $sha256 (param i32 i32 i32)))
 (table 0 anyfunc)
 (memory $0 32)
 (data (i32.const 4) "hello")
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64)
  (call $sha256
   (i32.const 4)
   (i32.const 5)
   (i32.const 5)
  )
 )
)
)=====";

static const char misaligned_const_ref_wast[] = R"=====(
(module
 (import "env" "sha256" (func $sha256 (param i32 i32 i32)))
 (import "env" "assert_sha256" (func $assert_sha256 (param i32 i32 i32)))
 (import "env" "memmove" (func $memmove (param i32 i32 i32) (result i32)))
 (table 0 anyfunc)
 (memory $0 32)
 (data (i32.const 4) "hello")
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64)
  (local $3 i32)
  (call $sha256
   (i32.const 4)
   (i32.const 5)
   (i32.const 16)
  )
  (set_local $3
   (call $memmove
    (i32.const 17)
    (i32.const 16)
    (i32.const 64) 
   )
  )
  (call $assert_sha256
   (i32.const 4)
   (i32.const 5)
   (i32.const 17)
  )
 )
)
)=====";

static const char entry_wast[] = R"=====(
(module
 (import "env" "require_auth" (func $require_auth (param i64)))
 (import "env" "eosio_assert" (func $eosio_assert (param i32 i32)))
 (import "env" "current_time" (func $current_time (result i64)))
 (table 0 anyfunc)
 (memory $0 1)
 (export "memory" (memory $0))
 (export "entry" (func $entry))
 (export "apply" (func $apply))
 (func $entry
  (block
   (i64.store offset=4
    (i32.const 0)
    (call $current_time)
   )
  )
 )
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64)
  (block
   (call $require_auth (i64.const 6121376101093867520))
   (call $eosio_assert
    (i64.eq
     (i64.load offset=4
      (i32.const 0)
     )
     (call $current_time)
    )
    (i32.const 0)
   )
  )
 )
 (start $entry)
)
)=====";

static const char entry_wast_2[] = R"=====(
(module
 (import "env" "require_auth" (func $require_auth (param i64)))
 (import "env" "eosio_assert" (func $eosio_assert (param i32 i32)))
 (import "env" "current_time" (func $current_time (result i64)))
 (table 0 anyfunc)
 (memory $0 1)
 (export "memory" (memory $0))
 (export "apply" (func $apply))
 (start $entry)
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64)
  (block
   (call $require_auth (i64.const 6121376101093867520))
   (call $eosio_assert
    (i64.eq
     (i64.load offset=4
      (i32.const 0)
     )
     (call $current_time)
    )
    (i32.const 0)
   )
  )
 )
 (func $entry
  (block
   (i64.store offset=4
    (i32.const 0)
    (call $current_time)
   )
  )
 )
)
)=====";

static const char entry_import_wast[] = R"=====(
(module
 (import "env" "abort" (func $abort))
 (export "apply" (func $apply))
 (start $abort)
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64))
)
)=====";

static const char entry_db_wast[] = R"=====(
(module
  (func $exit (import "env" "eosio_exit") (param i32))
  (func $db_store_i64 (import "env" "db_store_i64") (param i64 i64 i64 i64 i32 i32) (result i32))
  (func $current_receiver (import "env" "current_receiver") (result i64))
  (memory 1)
  (func $start
    (drop (call $db_store_i64 (i64.const 0) (i64.const 0) (call $current_receiver) (i64.const 0) (i32.const 0) (i32.const 0)))
  )
  (func (export "apply") (param i64 i64 i64)
    (call $exit (i32.const 0))
  )
  (start $start)
)
)=====";

static const char simple_no_memory_wast[] = R"=====(
(module
 (import "env" "require_auth" (func $require_auth (param i64)))
 (import "env" "memcpy" (func $memcpy (param i32 i32 i32) (result i32)))
 (table 0 anyfunc)
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64)
    (call $require_auth (i64.const 11323361180581363712))
    (drop
       (call $memcpy
          (i32.const 0)
          (i32.const 1024)
          (i32.const 1024)
       )
    )
 )
)
)=====";

static const char mutable_global_wast[] = R"=====(
(module
 (import "env" "require_auth" (func $require_auth (param i64)))
 (import "env" "eosio_assert" (func $eosio_assert (param i32 i32)))
 (table 0 anyfunc)
 (memory $0 1)
 (export "memory" (memory $0))
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64)
  (call $require_auth (i64.const 7235159549794234880))
  (if (i64.eq (get_local $2) (i64.const 0)) (then
    (set_global $g0 (i64.const 444))
    (return)
  ))
  (if (i64.eq (get_local $2) (i64.const 1)) (then
    (call $eosio_assert (i64.eq (get_global $g0) (i64.const 2)) (i32.const 0))
    (return)
  ))
  (call $eosio_assert (i32.const 0) (i32.const 0))
 )
 (global $g0 (mut i64) (i64.const 2))
)
)=====";

static const char biggest_memory_wast[] = R"=====(
(module
 (import "env" "eosio_assert" (func $$eosio_assert (param i32 i32)))
 (import "env" "require_auth" (func $$require_auth (param i64)))
 (table 0 anyfunc)
 (memory $$0 ${MAX_WASM_PAGES})
 (export "memory" (memory $$0))
 (export "apply" (func $$apply))
 (func $$apply (param $$0 i64) (param $$1 i64) (param $$2 i64)
  (call $$require_auth (i64.const 4294504710842351616))
  (call $$eosio_assert
   (i32.eq
     (grow_memory (i32.const 1))
     (i32.const -1)
   )
   (i32.const 0)
  )
 )
)
)=====";

static const char biggest_memory_variable_wast[] = R"=====(
(module
 (import "env" "eosio_assert" (func $$eosio_assert (param i32 i32)))
 (import "env" "require_auth" (func $$require_auth (param i64)))
 (table 0 anyfunc)
 (memory $$0 ${MAX_WASM_PAGES})
 (export "memory" (memory $$0))
 (export "apply" (func $$apply))
 (func $$apply (param $$0 i64) (param $$1 i64) (param $$2 i64)
  (call $$eosio_assert
   (i32.eq
     (grow_memory (i32.wrap/i64 (get_local 2)))
     (i32.const ${MAX_WASM_PAGES})
   )
   (i32.const 0)
  )
  (call $$eosio_assert
   (i32.eq
     (grow_memory (i32.const 1))
     (i32.const -1)
   )
   (i32.const 32)
  )
 )
 (data (i32.const 0) "failed grow_memory")
 (data (i32.const 32) "grow_memory unexpected success")
)
)=====";

static const char too_big_memory_wast[] = R"=====(
(module
 (table 0 anyfunc)
 (memory $$0 ${MAX_WASM_PAGES_PLUS_ONE})
 (export "memory" (memory $$0))
 (export "apply" (func $$apply))
 (func $$apply (param $$0 i64) (param $$1 i64) (param $$2 i64))
)
)=====";

static const char max_memory_wast[] = R"=====(
(module
 (import "env" "eosio_assert" (func $$eosio_assert (param i32 i32)))
 (import "env" "require_auth" (func $$require_auth (param i64)))
 (table 0 anyfunc)
 (memory $$0 ${INIT_WASM_PAGES} ${MAX_WASM_PAGES})
 (export "memory" (memory $$0))
 (export "apply" (func $$apply))
 (func $$apply (param $$0 i64) (param $$1 i64) (param $$2 i64)
  (call $$eosio_assert
   (i32.eq
     (grow_memory (i32.wrap/i64 (get_local 2)))
     (i32.const ${INIT_WASM_PAGES})
   )
   (i32.const 0)
  )
  (call $$eosio_assert
   (i32.eq
     (grow_memory (i32.const 1))
     (i32.const -1)
   )
   (i32.const 32)
  )
 )
 (data (i32.const 0) "failed grow_memory")
 (data (i32.const 32) "grow_memory unexpected success")
)
)=====";

static const char valid_sparse_table[] = R"=====(
(module
 (table 1024 anyfunc)
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64))
 (elem (i32.const 0) $apply)
 (elem (i32.const 1022) $apply $apply)
)
)=====";

static const char too_big_table[] = R"=====(
(module
 (table 1025 anyfunc)
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64))
 (elem (i32.const 0) $apply)
 (elem (i32.const 1022) $apply $apply)
)
)=====";

static const char variable_table[] = R"=====(
(module
 (table ${TABLE_SIZE} anyfunc)
 (func (export "apply") (param i64 i64 i64))
 (elem (i32.const 0) 0)
 (elem (i32.const ${TABLE_OFFSET}) 0 0)
)
)=====";

static const char memory_init_borderline[] = R"=====(
(module
 (memory $0 16)
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64))
 (data (i32.const 65532) "sup!")
)
)=====";

static const char memory_init_toolong[] = R"=====(
(module
 (memory $0 16)
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64))
 (data (i32.const 65533) "sup!")
)
)=====";

static const char memory_init_negative[] = R"=====(
(module
 (memory $0 16)
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64))
 (data (i32.const -1) "sup!")
)
)=====";

static const char memory_table_import[] = R"=====(
(module
 (table  (import "foo" "table") 10 anyfunc)
 (memory (import "nom" "memory") 0)
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64))
)
)=====";

static const char table_checker_wast[] = R"=====(
(module
 (import "env" "require_auth" (func $require_auth (param i64)))
 (import "env" "eosio_assert" (func $assert (param i32 i32)))
 (type $SIG$vj (func (param i64)))
 (table 1024 anyfunc)
 (memory $0 1)
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64)
   (call $require_auth (i64.const 14547189746360123392))
   (call_indirect $SIG$vj
     (i64.shr_u
       (get_local $2)
       (i64.const 32)
     )
     (i32.wrap/i64
       (get_local $2)
     )
   )
 )
 (func $apple (type $SIG$vj) (param $0 i64)
   (call $assert
     (i64.eq
       (get_local $0)
       (i64.const 555)
     )
     (i32.const 0)
   )
 )
 (func $bannna (type $SIG$vj) (param $0 i64)
   (call $assert
     (i64.eq
       (get_local $0)
       (i64.const 7777)
     )
     (i32.const 0)
   )
 )
 (elem (i32.const 0) $apple)
 (elem (i32.const 1022) $apple $bannna)
)
)=====";

static const char table_checker_proper_syntax_wast[] = R"=====(
(module
 (import "env" "require_auth" (func $require_auth (param i64)))
 (import "env" "eosio_assert" (func $assert (param i32 i32)))
 (import "env" "printi" (func $printi (param i64)))
 (type $SIG$vj (func (param i64)))
 (table 1024 anyfunc)
 (memory $0 1)
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64)
   (call $require_auth (i64.const 14547189746360123392))
   (call_indirect (type $SIG$vj)
     (i64.shr_u
       (get_local $2)
       (i64.const 32)
     )
     (i32.wrap/i64
       (get_local $2)
     )
   )
 )
 (func $apple (type $SIG$vj) (param $0 i64)
   (call $assert
     (i64.eq
       (get_local $0)
       (i64.const 555)
     )
     (i32.const 0)
   )
 )
 (func $bannna (type $SIG$vj) (param $0 i64)
   (call $assert
     (i64.eq
       (get_local $0)
       (i64.const 7777)
     )
     (i32.const 0)
   )
 )
 (elem (i32.const 0) $apple)
 (elem (i32.const 1022) $apple $bannna)
)
)=====";

static const char table_checker_small_wast[] = R"=====(
(module
 (import "env" "require_auth" (func $require_auth (param i64)))
 (import "env" "eosio_assert" (func $assert (param i32 i32)))
 (import "env" "printi" (func $printi (param i64)))
 (type $SIG$vj (func (param i64)))
 (table 128 anyfunc)
 (memory $0 1)
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64)
   (call $require_auth (i64.const 14547189746360123392))
   (call_indirect (type $SIG$vj)
     (i64.shr_u
       (get_local $2)
       (i64.const 32)
     )
     (i32.wrap/i64
       (get_local $2)
     )
   )
 )
 (func $apple (type $SIG$vj) (param $0 i64)
   (call $assert
     (i64.eq
       (get_local $0)
       (i64.const 555)
     )
     (i32.const 0)
   )
 )
 (elem (i32.const 0) $apple)
)
)=====";

static const char table_init_oob_wast[] = R"=====(
(module
 (type $mahsig (func (param i64) (param i64) (param i64)))
 (table 1024 anyfunc)
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64)
 )
 (elem (i32.const 1024) $apply)
)
)=====";

static const char table_init_oob_smaller_wast[] = R"=====(
(module
 (type $mahsig (func (param i64) (param i64) (param i64)))
 (table 620 anyfunc)
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64)
 )
 (elem (i32.const 700) $apply)
)
)=====";

static const char table_init_oob_no_table_wast[] = R"=====(
(module
 (type $mahsig (func (param i64) (param i64) (param i64)))
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64)
 )
 (elem (i32.const 0) $apply)
)
)=====";

static const char table_init_oob_empty_wast[] = R"=====(
(module
 (type $mahsig (func (param i64) (param i64) (param i64)))
 (table 1024 anyfunc)
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64)
 )
 (elem (i32.const 1025))
)
)=====";

static const char global_protection_none_get_wast[] = R"=====(
(module
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64)
   (drop (get_global 0))
 )
)
)=====";

static const char global_protection_some_get_wast[] = R"=====(
(module
 (global i32 (i32.const -11))
 (global i32 (i32.const 56))
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64)
   (drop (get_global 1))
   (drop (get_global 2))
 )
)
)=====";

static const char global_protection_none_set_wast[] = R"=====(
(module
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64)
   (set_global 0 (get_local 1))
 )
)
)=====";

static const char global_protection_some_set_wast[] = R"=====(
(module
 (global i64 (i64.const -11))
 (global i64 (i64.const 56))
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64)
   (set_global 2 (get_local 1))
 )
)
)=====";

static const std::vector<uint8_t> global_protection_okay_get_wasm{
   0x00, 'a', 's', 'm', 0x01, 0x00, 0x00, 0x00,
   0x01, 0x07, 0x01, 0x60, 0x03, 0x7e, 0x7e, 0x7e, 0x00,            //type section containing a function as void(i64,i64,i64)
   0x03, 0x02, 0x01, 0x00,                                          //a function

   0x06, 0x06, 0x01, 0x7f, 0x00, 0x41, 0x75, 0x0b,                  //global

   0x07, 0x09, 0x01, 0x05, 'a', 'p', 'p', 'l', 'y', 0x00, 0x00,     //export function 0 as "apply"
   0x0a, 0x07, 0x01,                                                //code section
   0x05, 0x00,                                                      //function body start with length 5; no locals
   0x23, 0x00,                                                      //get global 0
   0x1a,                                                            //drop
   0x0b                                                             //end
};

static const std::vector<uint8_t> global_protection_none_get_wasm{
   0x00, 'a', 's', 'm', 0x01, 0x00, 0x00, 0x00,
   0x01, 0x07, 0x01, 0x60, 0x03, 0x7e, 0x7e, 0x7e, 0x00,            //type section containing a function as void(i64,i64,i64)
   0x03, 0x02, 0x01, 0x00,                                          //a function

   0x07, 0x09, 0x01, 0x05, 'a', 'p', 'p', 'l', 'y', 0x00, 0x00,     //export function 0 as "apply"
   0x0a, 0x07, 0x01,                                                //code section
   0x05, 0x00,                                                      //function body start with length 5; no locals
   0x23, 0x00,                                                      //get global 0
   0x1a,                                                            //drop
   0x0b                                                             //end
};

static const std::vector<uint8_t> global_protection_some_get_wasm{
   0x00, 'a', 's', 'm', 0x01, 0x00, 0x00, 0x00,
   0x01, 0x07, 0x01, 0x60, 0x03, 0x7e, 0x7e, 0x7e, 0x00,            //type section containing a function as void(i64,i64,i64)
   0x03, 0x02, 0x01, 0x00,                                          //a function

   0x06, 0x06, 0x01, 0x7f, 0x00, 0x41, 0x75, 0x0b,                  //global

   0x07, 0x09, 0x01, 0x05, 'a', 'p', 'p', 'l', 'y', 0x00, 0x00,     //export function 0 as "apply"
   0x0a, 0x07, 0x01,                                                //code section
   0x05, 0x00,                                                      //function body start with length 5; no locals
   0x23, 0x01,                                                      //get global 1
   0x1a,                                                            //drop
   0x0b                                                             //end
};

static const std::vector<uint8_t> global_protection_okay_set_wasm{
   0x00, 'a', 's', 'm', 0x01, 0x00, 0x00, 0x00,
   0x01, 0x07, 0x01, 0x60, 0x03, 0x7e, 0x7e, 0x7e, 0x00,            //type section containing a function as void(i64,i64,i64)
   0x03, 0x02, 0x01, 0x00,                                          //a function

   0x06, 0x06, 0x01, 0x7e, 0x01, 0x42, 0x75, 0x0b,                  //global.. this time with i64 & global mutablity

   0x07, 0x09, 0x01, 0x05, 'a', 'p', 'p', 'l', 'y', 0x00, 0x00,     //export function 0 as "apply"
   0x0a, 0x08, 0x01,                                                //code section
   0x06, 0x00,                                                      //function body start with length 6; no locals
   0x20, 0x00,                                                      //get local 0
   0x24, 0x00,                                                      //set global 0
   0x0b                                                             //end
};

static const std::vector<uint8_t> global_protection_some_set_wasm{
   0x00, 'a', 's', 'm', 0x01, 0x00, 0x00, 0x00,
   0x01, 0x07, 0x01, 0x60, 0x03, 0x7e, 0x7e, 0x7e, 0x00,            //type section containing a function as void(i64,i64,i64)
   0x03, 0x02, 0x01, 0x00,                                          //a function

   0x06, 0x06, 0x01, 0x7e, 0x01, 0x42, 0x75, 0x0b,                  //global.. this time with i64 & global mutablity

   0x07, 0x09, 0x01, 0x05, 'a', 'p', 'p', 'l', 'y', 0x00, 0x00,     //export function 0 as "apply"
   0x0a, 0x08, 0x01,                                                //code section
   0x06, 0x00,                                                      //function body start with length 6; no locals
   0x20, 0x00,                                                      //get local 0
   0x24, 0x01,                                                      //set global 1
   0x0b                                                             //end
};

static const char no_apply_wast[] = R"=====(
(module
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64))
)
)=====";

static const char no_apply_2_wast[] = R"=====(
(module
  (type (;0;) (func))
  (type (;1;) (func (result i64)))
  (type (;2;) (func (param i64 i64)))
  (type (;3;) (func (param i64)))
  (type (;4;) (func (param i32 i32)))
  (type (;5;) (func (result i32)))
  (type (;6;) (func (param i32 i32) (result i32)))
  (type (;7;) (func (param i32 i32 i32) (result i32)))
  (type (;8;) (func (param i64 i64 i64)))
  (func (;0;) (type 8) (param i64 i64 i64))
  (global (;0;) f32 (f32.const -0x1.8008p+2 (;=-6.00049;)))
  (export "llp/y" (func 0)))
)=====";

static const char no_apply_3_wast[] = R"=====(
(module
  (type (;0;) (func (param i64)))
  (type (;1;) (func (param i64)))
  (type (;2;) (func (param i64)))
  (type (;3;) (func (param i64 i64 i64)))
  (global (;0;) f32 (f32.const -0x1.8008p+2 (;=-6.00049;)))
  (global (;1;) f32 (f32.const -0x1.8008p+2 (;=-6.00049;)))
  (global (;2;) f32 (f32.const -0x1.8008p+2 (;=-6.00049;)))
  (global (;3;) f32 (f32.const -0x1.8008p+2 (;=-6.00049;)))
  (global (;4;) f32 (f32.const -0x1.8008p+2 (;=-6.00049;)))
  (global (;5;) f32 (f32.const -0x1.8008p+2 (;=-6.00049;)))
  (func (;0;) (type 3) (param i64 i64 i64))
  (export "apply" (global 5))
)
)=====";

static const char apply_wrong_signature_wast[] = R"=====(
(module
 (export "apply" (func $apply))
 (func $apply (param $0 i64) (param $1 f64))
)
)=====";

static const char import_injected_wast[] =                                            \
"(module"                                                                             \
" (export \"apply\" (func $apply))"                                                   \
" (import \"" EOSIO_INJECTED_MODULE_NAME "\" \"checktime\" (func $inj (param i32)))"  \
" (func $apply (param $0 i64) (param $1 i64) (param $2 i64))"                         \
")";

static const char import_wrong_signature_wast[] = R"=====(
(module
 (import "env" "eosio_assert" (func (param i32 i64)))
 (func (export "apply") (param i64 i64 i64))
)
)=====";

static const char memory_growth_memset_store[] = R"=====(
(module
 (export "apply" (func $apply))
 (memory $0 1)
 (func $apply (param $0 i64)(param $1 i64)(param $2 i64)
    (drop (grow_memory (i32.const 2)))
    (i32.store (i32.const 80000) (i32.const 2))
    (i32.store (i32.const 140000) (i32.const 3))
 )
)
)=====";

static const char memory_growth_memset_test[] = R"=====(
(module
 (export "apply" (func $apply))
 (import "env" "eosio_assert" (func $eosio_assert (param i32 i32)))
 (memory $0 1)
 (func $apply (param $0 i64)(param $1 i64)(param $2 i64)
   (drop (grow_memory (i32.const 2)))
   (call $eosio_assert
     (i32.eq
       (i32.load offset=80000 (i32.const 0))
       (i32.const 0)
     )
     (i32.const 0)
   )
   (call $eosio_assert
     (i32.eq
       (i32.load offset=140000 (i32.const 0))
       (i32.const 0)
     )
     (i32.const 0)
   )
 )
)
)=====";

static const char large_maligned_host_ptr[] = R"=====(
(module
 (export "apply" (func $$apply))
 (import "env" "get_active_producers" (func $$get_active_producers (param i32 i32) (result i32)))
 (memory $$0 ${MAX_WASM_PAGES})
 (func $$apply (param i64) (param i64) (param i64)
   (drop (call $$get_active_producers
     (i32.const 1)
     (i32.const ${MAX_NAME_ARRAY})
   ))
 )
)
)=====";

static const char depth_assert_wasm[] = R"=====(
(module
 (export "apply" (func $$apply))
 (func $$apply (param $$0 i64) (param $$1 i64) (param $$2 i64)
  (if (i64.eq (get_global $$depth) (i64.const 0)) (then
    (return)
  ))
  (set_global $$depth
   (i64.sub
    (get_global $$depth)
    (i64.const 1)
   )
  )
  (call $$apply
   (get_local $$0)
   (get_local $$1)
   (get_local $$2)
  )
 )
 (global $$depth (mut i64) (i64.const ${MAX_DEPTH}))
)
)=====";

static const char depth_assert_intrinsic[] = R"=====(
(module
 (import "env" "current_time" (func $$current_time (result i64)))
 (export "apply" (func $$apply))
 (func $$apply (param $$0 i64) (param $$1 i64) (param $$2 i64)
  (if (i64.eq (get_global $$depth) (i64.const 1)) (then
    (drop (call $$current_time))
    (return)
  ))
  (set_global $$depth
   (i64.sub
    (get_global $$depth)
    (i64.const 1)
   )
  )
  (call $$apply
   (get_local $$0)
   (get_local $$1)
   (get_local $$2)
  )
 )
 (global $$depth (mut i64) (i64.const ${MAX_DEPTH}))
)
)=====";

static const char depth_assert_wasm_float[] = R"=====(
(module
 (export "apply" (func $$apply))
 (func $$apply (param $$0 i64) (param $$1 i64) (param $$2 i64)
  (set_global $$mcfloaty
   (f64.mul
    (get_global $$mcfloaty)
    (f64.const 3.1415)
   )
  )
  (if (i64.eq (get_global $$depth) (i64.const 0)) (then
    (return)
  ))
  (set_global $$depth
   (i64.sub
    (get_global $$depth)
    (i64.const 1)
   )
  )
  (call $$apply
   (get_local $$0)
   (get_local $$1)
   (get_local $$2)
  )
 )
 (global $$depth (mut i64) (i64.const ${MAX_DEPTH}))
 (global $$mcfloaty (mut f64) (f64.const 3.14))
)
)=====";

static const std::vector<uint8_t> varuint_memory_flags{
  0x00, 'a', 's', 'm', 0x01, 0x00, 0x00, 0x00,
  0x01, 0x07, 0x01, 0x60, 0x03, 0x7e, 0x7e, 0x7e, 0x00, // types
  0x03, 0x02, 0x01, 0x00, // functions
  0x04, 0x08, 0x01, 0x70, 0x80, 0x02, 0x80, 0x80, 0x80, 0x00, // memory with flags varuint(0x80 0x02) -> 0x2
  0x07, 0x09, 0x01, 0x05, 'a', 'p', 'p', 'l', 'y', 0x00, 0x00, // exports
  0x0a, 0x04, 0x01, 0x02, 0x00, 0x0b // code
};

static const char zero_memory_do_nothing[] = R"=====(
(module
 (export "apply" (func $apply))
 (memory $0 0)
 (func $apply (param i64) (param i64) (param i64)
 )
)
)=====";

static const char zero_memory_load[] = R"=====(
(module
 (export "apply" (func $apply))
 (memory $0 0)
 (func $apply (param i64) (param i64) (param i64)
   (drop (i64.load (i32.const 0)))
 )
)
)=====";

static const char zero_memory_intrinsic[] = R"=====(
(module
 (export "apply" (func $apply))
 (import "env" "read_transaction" (func $read_transaction (param i32 i32) (result i32)))
 (memory $0 0)
 (func $apply (param i64) (param i64) (param i64)
   (drop (call $read_transaction (i32.const 16) (i32.const 0)))
 )
)
)=====";

static const char zero_memory_grow[] = R"=====(
(module
 (export "apply" (func $apply))
 (memory $0 0)
 (func $apply (param i64) (param i64) (param i64)
   (drop (grow_memory (i32.const 1)))
   (drop (i64.load (i32.const 0)))
 )
)
)=====";

static const char zero_memory_grow_hi[] = R"=====(
(module
 (export "apply" (func $apply))
 (memory $0 0)
 (func $apply (param i64) (param i64) (param i64)
   (drop (grow_memory (i32.const 1)))
   (drop (i64.load (i32.const 70000)))
 )
)
)=====";

static const char exit_in_start_wast[] = R"=====(
(module
 (import "env" "abort" (func $abort))
 (import "env" "eosio_exit" (func $eosio_exit (param i32)))
 (export "apply" (func $apply))
 (start $dothedew)
 (func $dothedew
   (call $eosio_exit (i32.const 0))
 )
 (func $apply (param $0 i64) (param $1 i64) (param $2 i64)
   (call $abort)
 )
)
)=====";

static const char negative_memory_grow_wast[] = R"=====(
(module
 (memory 1)
 (func (export "apply") (param i64 i64 i64)
  (block
   (drop (grow_memory (i32.const 1)))
   (i32.eq (grow_memory (i32.const -1)) (i32.const 2))
   (br_if 0)
   (unreachable)
  )
  (block
   (drop (grow_memory (i32.const 2)))
   (i32.eq (grow_memory (i32.const -3)) (i32.const 3))
   (br_if 0)
   (unreachable)
  )
  (block
   (drop (grow_memory (i32.const 1)))
   (i32.store (i32.const 0) (i32.const -1))
   (i32.store (i32.const 65532) (i32.const -1))
   (grow_memory (i32.const -1))
   (grow_memory (i32.const 1))
   (i32.and (i32.eq (i32.load (i32.const 0)) (i32.const 0))
            (i32.eq (i32.load (i32.const 65532)) (i32.const 0)))
   (br_if 0)
   (unreachable)
  )
  (block
   (i32.eq (grow_memory (i32.const -2)) (i32.const -1))
   (br_if 0)
   (unreachable)
  )
 )
)
)=====";

static const char negative_memory_grow_trap_wast[] = R"=====(
(module
 (memory 1)
 (func (export "apply") (param i64 i64 i64)
  (block
   (drop (grow_memory (i32.const -1)))
   (drop (i32.load (i32.const 0)))
  )
 )
)
)=====";

static const char set_jumbo_row_wast[] = R"=====(
(module
   (import "env" "db_store_i64" (func $db_store_i64 (param i64 i64 i64 i64 i32 i32) (result i32)))
   (memory $0 528)
   (export "apply" (func $apply))
   (func $apply (param $receiver i64) (param $account i64) (param $action_name i64)
      (drop (call $db_store_i64 (get_local $receiver) (get_local $receiver) (get_local $receiver) (i64.const 0) (i32.const 1) (i32.const 34603007)))
   )
)
)=====";

static const char divmod_host_function_overflow_wast[] = R"=====(
(module
   (import "env" "__divti3" (func $__divti3 (param $ret_ptr i32) (param $la i64) (param $ha i64) (param $lb i64) (param $hb i64)))
   (import "env" "__modti3" (func $__modti3 (param $ret_ptr i32) (param $la i64) (param $ha i64) (param $lb i64) (param $hb i64)))

   (memory $0 1)
   (export "apply" (func $apply))

   (func $apply (param $receiver i64) (param $account i64) (param $action_name i64)
      (call $__divti3 (i32.const 8)
         (i64.const 0)                  (i64.const 0x8000000000000000)   ;; bytes: 0x00000000000000000000000000000080 (INT128_MIN)
         (i64.const 0xffffffffffffffff) (i64.const 0xffffffffffffffff)   ;; -1
      )
      ;;should still be bytes 00000000000000000000000000000080
      (if (i64.ne (i64.load (i32.const 8)) (i64.const 0)) (then
         (unreachable)
      ))
      (if (i64.ne (i64.load (i32.const 16)) (i64.const 0x8000000000000000)) (then
         (unreachable)
      ))

      (call $__modti3 (i32.const 8)
         (i64.const 0)                  (i64.const 0x8000000000000000)   ;; bytes: 0x00000000000000000000000000000080 (INT128_MIN)
         (i64.const 0xffffffffffffffff) (i64.const 0xffffffffffffffff)   ;; -1
      )
      ;;should still be all 00s
      (if (i64.ne (i64.load (i32.const 8)) (i64.const 0)) (then
         (unreachable)
      ))
      (if (i64.ne (i64.load (i32.const 16)) (i64.const 0)) (then
         (unreachable)
      ))
   )
)
)=====";

static const char small_memcpy_const_dstsrc_wastfmt[] = R"=====(
(module
   (import "env" "memcpy" (func $$memcpy (param i32 i32 i32) (result i32)))
   (import "env" "memcmp" (func $$memcmp (param i32 i32 i32) (result i32)))
   (export "apply" (func $$apply))
   (memory 1)
   (func $$apply (param i64) (param i64) (param i64)
      ;; do copy and check that return value is dst
      (if (i32.ne (call $$memcpy (i32.const 256) (i32.const 64) (i32.const ${COPY_SIZE})) (i32.const 256)) (then unreachable))

      ;; validate copied region
      (if (i32.ne (call $$memcmp (i32.const 256) (i32.const 64) (i32.const ${COPY_SIZE})) (i32.const 0)) (then unreachable))

      ;; check the 4 bytes before and and after the copied region and expect them to still be 0x0
      (if (i32.ne (i32.load (i32.const 252)) (i32.const 0)) (then unreachable))
      (if (i32.ne (i32.load (i32.add (i32.const 256) (i32.const ${COPY_SIZE}))) (i32.const 0)) (then unreachable))
   )
   (data (i32.const 64) "1234567890-abcdefghijklmnopqrstuvwxyz_ABCDEFGHIJKLMNOPQRSTUVWXYZ")
)
)=====";

static const char small_memcpy_var_dstsrc_wastfmt[] = R"=====(
(module
   (import "env" "memcpy" (func $$memcpy (param i32 i32 i32) (result i32)))
   (import "env" "memcmp" (func $$memcmp (param i32 i32 i32) (result i32)))
   (export "apply" (func $$apply))
   (memory 1)
   (func $$doit (param $$dst i32) (param $$src i32)
      ;; do copy and check that return value is dst
      (if (i32.ne (call $$memcpy (get_local $$dst) (get_local $$src) (i32.const ${COPY_SIZE})) (get_local $$dst)) (then unreachable))

      ;; validate copied region
      (if (i32.ne (call $$memcmp (get_local $$dst) (get_local $$src) (i32.const ${COPY_SIZE})) (i32.const 0)) (then unreachable))

      ;; check the 4 bytes before and and after the copied region and expect them to still be 0x0
      (if (i32.ne (i32.load (i32.sub (get_local $$dst) (i32.const 4))) (i32.const 0)) (then unreachable))
      (if (i32.ne (i32.load (i32.add (get_local $$dst) (i32.const ${COPY_SIZE}))) (i32.const 0)) (then unreachable))
   )
   (func $$apply (param i64) (param i64) (param i64)
      (call $$doit (i32.const 256) (i32.const 64))
   )
   (data (i32.const 64) "1234567890-abcdefghijklmnopqrstuvwxyz_ABCDEFGHIJKLMNOPQRSTUVWXYZ")
)
)=====";

static const char small_memcpy_overlapenddst_wast[] = R"=====(
(module
   (import "env" "memcpy" (func $memcpy (param i32 i32 i32) (result i32)))
   (export "apply" (func $apply))
   (memory 1)
   (func $apply (param i64) (param i64) (param i64)
      (drop (call $memcpy (i32.const 65532) (i32.const 64) (i32.const 8)))
   )

   (data (i32.const 64) "1234567890-abcdefghijklmnopqrstuvwxyz_ABCDEFGHIJKLMNOPQRSTUVWXYZ")
)
)=====";

static const char small_memcpy_overlapendsrc_wast[] = R"=====(
(module
   (import "env" "memcpy" (func $memcpy (param i32 i32 i32) (result i32)))
   (export "apply" (func $apply))
   (memory 1)
   (func $apply (param i64) (param i64) (param i64)
      (drop (call $memcpy (i32.const 4) (i32.const 65532) (i32.const 8)))
   )
)
)=====";

static const char small_memcpy_high_wast[] = R"=====(
(module
   (import "env" "memcpy" (func $memcpy (param i32 i32 i32) (result i32)))
   (export "apply" (func $apply))
   (memory 1)
   (func $apply (param i64) (param i64) (param i64)
      (drop (call $memcpy (i32.const 4294967295) (i32.const 4294967200) (i32.const 8)))
   )
)
)=====";