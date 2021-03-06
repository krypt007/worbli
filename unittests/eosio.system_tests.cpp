#include <boost/test/unit_test.hpp>
#include <eosio/chain/contract_table_objects.hpp>
#include <eosio/chain/global_property_object.hpp>
#include <eosio/chain/resource_limits.hpp>
#include <eosio/chain/wast_to_wasm.hpp>

#include <Runtime/Runtime.h>

#include "eosio_system_tester.hpp"

using namespace eosio_system;

BOOST_AUTO_TEST_SUITE(eosio_system_tests)

BOOST_FIXTURE_TEST_CASE( buysell, eosio_system_tester ) try {

   BOOST_REQUIRE_EQUAL( core_from_string("0.0000"), get_balance( "alice1111111" ) );

   transfer( "eosio", "alice1111111", core_from_string("1000.0000"), "eosio" );
   BOOST_REQUIRE_EQUAL( success(), stake( "eosio", "alice1111111", core_from_string("200.0000"), core_from_string("100.0000") ) );

   auto total = get_total_stake( "alice1111111" );
   auto init_bytes =  total["ram_bytes"].as_uint64();

   BOOST_REQUIRE_EQUAL( success(), buyram( "alice1111111", "alice1111111", core_from_string("200.0000") ) );
   BOOST_REQUIRE_EQUAL( core_from_string("800.0000"), get_balance( "alice1111111" ) );

   total = get_total_stake( "alice1111111" );
   auto bytes = total["ram_bytes"].as_uint64();
   auto bought_bytes = bytes - init_bytes;
   wdump((init_bytes)(bought_bytes)(bytes) );

   BOOST_REQUIRE_EQUAL( true, 0 < bought_bytes );

   BOOST_REQUIRE_EQUAL( success(), sellram( "alice1111111", bought_bytes ) );

   auto refund = get_refund_request( "alice1111111" );

   BOOST_REQUIRE_EQUAL( core_from_string("1000.0000"), get_balance( "alice1111111" ) + refund["ram_amount"].as<asset>());
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( true, total["ram_bytes"].as_uint64() == init_bytes );

   transfer( "eosio", "alice1111111", core_from_string("100000000.0000"), "eosio" );

   BOOST_REQUIRE_EQUAL( core_from_string("100001000.0000"), get_balance( "alice1111111" ) + refund["ram_amount"].as<asset>() );
   // alice buys ram for 10000000.0000, 0.5% = 50000.0000 go to ramfee
   // after fee 9950000.0000 go to bought bytes
   // when selling back bought bytes, pay 0.5% fee and get back 99.5% of 9950000.0000 = 9900250.0000
   // expected account after that is 90000998.0049 + 9900250.0000 = 99901248.0049 with a difference
   // of order 0.0001 due to rounding errors
   BOOST_REQUIRE_EQUAL( success(), buyram( "alice1111111", "alice1111111", core_from_string("10000000.0000") ) );
   BOOST_REQUIRE_EQUAL( core_from_string("90001000.0000"), get_balance( "alice1111111" ) );

   total = get_total_stake( "alice1111111" );
   bytes = total["ram_bytes"].as_uint64();
   bought_bytes = bytes - init_bytes;
   wdump((init_bytes)(bought_bytes)(bytes) );

   BOOST_REQUIRE_EQUAL( success(), sellram( "alice1111111", bought_bytes ) );
   total = get_total_stake( "alice1111111" );

   bytes = total["ram_bytes"].as_uint64();
   bought_bytes = bytes - init_bytes;
   wdump((init_bytes)(bought_bytes)(bytes) );

   BOOST_REQUIRE_EQUAL( true, total["ram_bytes"].as_uint64() == init_bytes );

   refund = get_refund_request( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("100001000.6144"), get_balance( "alice1111111" ) + refund["ram_amount"].as<asset>());

   BOOST_REQUIRE_EQUAL( success(), buyram( "alice1111111", "alice1111111", core_from_string("100.0000") ) );
   BOOST_REQUIRE_EQUAL( success(), buyram( "alice1111111", "alice1111111", core_from_string("100.0000") ) );
   BOOST_REQUIRE_EQUAL( success(), buyram( "alice1111111", "alice1111111", core_from_string("100.0000") ) );
   BOOST_REQUIRE_EQUAL( success(), buyram( "alice1111111", "alice1111111", core_from_string("100.0000") ) );
   BOOST_REQUIRE_EQUAL( success(), buyram( "alice1111111", "alice1111111", core_from_string("100.0000") ) );
   BOOST_REQUIRE_EQUAL( success(), buyram( "alice1111111", "alice1111111", core_from_string("10.0000") ) );
   BOOST_REQUIRE_EQUAL( success(), buyram( "alice1111111", "alice1111111", core_from_string("10.0000") ) );
   BOOST_REQUIRE_EQUAL( success(), buyram( "alice1111111", "alice1111111", core_from_string("10.0000") ) );
   BOOST_REQUIRE_EQUAL( success(), buyram( "alice1111111", "alice1111111", core_from_string("30.0000") ) );

   refund = get_refund_request( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("100000440.6144"), get_balance( "alice1111111" ) + refund["ram_amount"].as<asset>());

   auto newtotal = get_total_stake( "alice1111111" );

   auto newbytes = newtotal["ram_bytes"].as_uint64();
   bought_bytes = newbytes - bytes;
   wdump((newbytes)(bytes)(bought_bytes) );

   BOOST_REQUIRE_EQUAL( success(), sellram( "alice1111111", bought_bytes ) );
   refund = get_refund_request( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("100001000.3938"), get_balance( "alice1111111" ) + refund["ram_amount"].as<asset>());


   newtotal = get_total_stake( "alice1111111" );
   auto startbytes = newtotal["ram_bytes"].as_uint64();

   BOOST_REQUIRE_EQUAL( success(), buyram( "alice1111111", "alice1111111", core_from_string("10000000.0000") ) );
   BOOST_REQUIRE_EQUAL( success(), buyram( "alice1111111", "alice1111111", core_from_string("10000000.0000") ) );
   BOOST_REQUIRE_EQUAL( success(), buyram( "alice1111111", "alice1111111", core_from_string("10000000.0000") ) );
   BOOST_REQUIRE_EQUAL( success(), buyram( "alice1111111", "alice1111111", core_from_string("10000000.0000") ) );
   BOOST_REQUIRE_EQUAL( success(), buyram( "alice1111111", "alice1111111", core_from_string("10000000.0000") ) );
   BOOST_REQUIRE_EQUAL( success(), buyram( "alice1111111", "alice1111111", core_from_string("100000.0000") ) );
   BOOST_REQUIRE_EQUAL( success(), buyram( "alice1111111", "alice1111111", core_from_string("100000.0000") ) );
   BOOST_REQUIRE_EQUAL( success(), buyram( "alice1111111", "alice1111111", core_from_string("100000.0000") ) );
   BOOST_REQUIRE_EQUAL( success(), buyram( "alice1111111", "alice1111111", core_from_string("300000.0000") ) );

   // should be no refund so we don't include it in the assert.
   BOOST_REQUIRE_EQUAL( core_from_string("49401000.3938"), get_balance( "alice1111111" ));

   auto finaltotal = get_total_stake( "alice1111111" );
   auto endbytes = finaltotal["ram_bytes"].as_uint64();

   bought_bytes = endbytes - startbytes;
   wdump((startbytes)(endbytes)(bought_bytes) );

   BOOST_REQUIRE_EQUAL( success(), sellram( "alice1111111", bought_bytes ) );
   refund = get_refund_request( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("100000998.6530"), get_balance( "alice1111111" ) + refund["ram_amount"].as<asset>());

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( stake_unstake, eosio_system_tester ) try {
   //cross_15_percent_threshold();

   produce_blocks( 10 );
   produce_block( fc::hours(3*24) );

   BOOST_REQUIRE_EQUAL( core_from_string("0.0000"), get_balance( "alice1111111" ) );
   transfer( "eosio", "alice1111111", core_from_string("1000.0000"), "eosio" );

   BOOST_REQUIRE_EQUAL( core_from_string("1000.0000"), get_balance( "alice1111111" ) );
   BOOST_REQUIRE_EQUAL( success(), stake( "eosio", "alice1111111", core_from_string("200.0000"), core_from_string("100.0000") ) );

   auto total = get_total_stake("alice1111111");
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["cpu_weight"].as<asset>());

   const auto init_eosio_stake_balance = get_balance( N(eosio.stake) );
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "alice1111111", core_from_string("200.0000"), core_from_string("100.0000") ) );
   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );
   BOOST_REQUIRE_EQUAL( init_eosio_stake_balance + core_from_string("300.0000"), get_balance( N(eosio.stake) ) );
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice1111111", "alice1111111", core_from_string("200.0000"), core_from_string("100.0000") ) );
   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );

   produce_block( fc::hours(3*24-1) );
   produce_blocks(1);
   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );
   BOOST_REQUIRE_EQUAL( init_eosio_stake_balance + core_from_string("300.0000"), get_balance( N(eosio.stake) ) );
   //after 3 days funds should be released
   produce_block( fc::hours(1) );
   produce_blocks(1);
   BOOST_REQUIRE_EQUAL( core_from_string("1000.0000"), get_balance( "alice1111111" ) );
   BOOST_REQUIRE_EQUAL( init_eosio_stake_balance, get_balance( N(eosio.stake) ) );

   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "bob111111111", core_from_string("200.0000"), core_from_string("100.0000") ) );
   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );
   total = get_total_stake("bob111111111");
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["cpu_weight"].as<asset>());

   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000").get_amount(), total["net_weight"].as<asset>().get_amount() );
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000").get_amount(), total["cpu_weight"].as<asset>().get_amount() );

   auto bytes = total["ram_bytes"].as_uint64();
   BOOST_REQUIRE_EQUAL( true, 0 < bytes );

   //unstake from bob111111111
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice1111111", "bob111111111", core_from_string("200.0000"), core_from_string("100.0000") ) );
   total = get_total_stake("bob111111111");
   BOOST_REQUIRE_EQUAL( core_from_string("10.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("10.0000"), total["cpu_weight"].as<asset>());
   produce_block( fc::hours(3*24-1) );
   produce_blocks(1);
   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );
   //after 3 days funds should be released
   produce_block( fc::hours(1) );
   produce_blocks(1);

   produce_blocks(1);
   BOOST_REQUIRE_EQUAL( core_from_string("1000.0000"), get_balance( "alice1111111" ) );
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( stake_unstake_with_transfer, eosio_system_tester ) try {
   //cross_15_percent_threshold();

   issue( "eosio", core_from_string("1000.0000"), config::system_account_name );
   issue( "eosio.stake", core_from_string("1000.0000"), config::system_account_name );
   BOOST_REQUIRE_EQUAL( core_from_string("0.0000"), get_balance( "alice1111111" ) );

   //eosio stakes for alice with transfer flag

   transfer( "eosio", "bob111111111", core_from_string("1000.0000"), "eosio" );
   BOOST_REQUIRE_EQUAL( success(), stake_with_transfer( "bob111111111", "alice1111111", core_from_string("200.0000"), core_from_string("100.0000") ) );

   //check that alice has both bandwidth and voting power
   auto total = get_total_stake("alice1111111");
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["cpu_weight"].as<asset>());

   BOOST_REQUIRE_EQUAL( core_from_string("0.0000"), get_balance( "alice1111111" ) );

   //alice stakes for herself
   transfer( "eosio", "alice1111111", core_from_string("1000.0000"), "eosio" );
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "alice1111111", core_from_string("200.0000"), core_from_string("100.0000") ) );
   //now alice's stake should be equal to transfered from eosio + own stake
   total = get_total_stake("alice1111111");
   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );
   BOOST_REQUIRE_EQUAL( core_from_string("410.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000"), total["cpu_weight"].as<asset>());

   //alice can unstake everything (including what was transfered)
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice1111111", "alice1111111", core_from_string("400.0000"), core_from_string("200.0000") ) );
   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );

   produce_block( fc::hours(3*24-1) );
   produce_blocks(1);
   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );
   //after 3 days funds should be released

   produce_block( fc::hours(1) );
   produce_blocks(1);

   BOOST_REQUIRE_EQUAL( core_from_string("1300.0000"), get_balance( "alice1111111" ) );

   //stake should be equal to what was staked in constructor, voting power should be 0
   total = get_total_stake("alice1111111");
   BOOST_REQUIRE_EQUAL( core_from_string("10.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("10.0000"), total["cpu_weight"].as<asset>());

   // Now alice stakes to bob with transfer flag
   BOOST_REQUIRE_EQUAL( success(), stake_with_transfer( "alice1111111", "bob111111111", core_from_string("100.0000"), core_from_string("100.0000") ) );

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( stake_to_self_with_transfer, eosio_system_tester ) try {
   //cross_15_percent_threshold();

   BOOST_REQUIRE_EQUAL( core_from_string("0.0000"), get_balance( "alice1111111" ) );
   transfer( "eosio", "alice1111111", core_from_string("1000.0000"), "eosio" );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("cannot use transfer flag if delegating to self"),
                        stake_with_transfer( "alice1111111", "alice1111111", core_from_string("200.0000"), core_from_string("100.0000") )
   );

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( stake_while_pending_refund, eosio_system_tester ) try {
   //cross_15_percent_threshold();

   issue( "eosio", core_from_string("1000.0000"), config::system_account_name );
   issue( "eosio.stake", core_from_string("1000.0000"), config::system_account_name );
   BOOST_REQUIRE_EQUAL( core_from_string("0.0000"), get_balance( "alice1111111" ) );

   //eosio stakes for alice with transfer flag

   transfer( "eosio", "bob111111111", core_from_string("1000.0000"), "eosio" );
   BOOST_REQUIRE_EQUAL( success(), stake_with_transfer( "bob111111111", "alice1111111", core_from_string("200.0000"), core_from_string("100.0000") ) );

   //check that alice has both bandwidth and voting power
   auto total = get_total_stake("alice1111111");
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["cpu_weight"].as<asset>());

   BOOST_REQUIRE_EQUAL( core_from_string("0.0000"), get_balance( "alice1111111" ) );

   //alice stakes for herself
   transfer( "eosio", "alice1111111", core_from_string("1000.0000"), "eosio" );
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "alice1111111", core_from_string("200.0000"), core_from_string("100.0000") ) );
   //now alice's stake should be equal to transfered from eosio + own stake
   total = get_total_stake("alice1111111");
   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );
   BOOST_REQUIRE_EQUAL( core_from_string("410.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000"), total["cpu_weight"].as<asset>());

   //alice can unstake everything (including what was transfered)
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice1111111", "alice1111111", core_from_string("400.0000"), core_from_string("200.0000") ) );
   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );

   produce_block( fc::hours(3*24-1) );
   produce_blocks(1);
   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );
   //after 3 days funds should be released

   produce_block( fc::hours(1) );
   produce_blocks(1);

   BOOST_REQUIRE_EQUAL( core_from_string("1300.0000"), get_balance( "alice1111111" ) );

   //stake should be equal to what was staked in constructor, voting power should be 0
   total = get_total_stake("alice1111111");
   BOOST_REQUIRE_EQUAL( core_from_string("10.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("10.0000"), total["cpu_weight"].as<asset>());

   // Now alice stakes to bob with transfer flag
   BOOST_REQUIRE_EQUAL( success(), stake_with_transfer( "alice1111111", "bob111111111", core_from_string("100.0000"), core_from_string("100.0000") ) );

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( fail_without_auth, eosio_system_tester ) try {
   //cross_15_percent_threshold();

   issue( "alice1111111", core_from_string("1000.0000"),  config::system_account_name );

   BOOST_REQUIRE_EQUAL( success(), stake( "eosio", "alice1111111", core_from_string("2000.0000"), core_from_string("1000.0000") ) );
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "bob111111111", core_from_string("10.0000"), core_from_string("10.0000") ) );

   BOOST_REQUIRE_EQUAL( error("missing authority of alice1111111"),
                        push_action( N(alice1111111), N(delegatebw), mvo()
                                    ("from",     "alice1111111")
                                    ("receiver", "bob111111111")
                                    ("stake_net_quantity", core_from_string("10.0000"))
                                    ("stake_cpu_quantity", core_from_string("10.0000"))
                                    ("transfer", 0 )
                                    ,false
                        )
   );

   BOOST_REQUIRE_EQUAL( error("missing authority of alice1111111"),
                        push_action(N(alice1111111), N(undelegatebw), mvo()
                                    ("from",     "alice1111111")
                                    ("receiver", "bob111111111")
                                    ("unstake_net_quantity", core_from_string("200.0000"))
                                    ("unstake_cpu_quantity", core_from_string("100.0000"))
                                    ("transfer", 0 )
                                    ,false
                        )
   );
   //REQUIRE_MATCHING_OBJECT( , get_voter_info( "alice1111111" ) );
} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( stake_negative, eosio_system_tester ) try {
   issue( "alice1111111", core_from_string("1000.0000"),  config::system_account_name );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("must stake a positive amount"),
                        stake( "alice1111111", core_from_string("-0.0001"), core_from_string("0.0000") )
   );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("must stake a positive amount"),
                        stake( "alice1111111", core_from_string("0.0000"), core_from_string("-0.0001") )
   );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("must stake a positive amount"),
                        stake( "alice1111111", core_from_string("00.0000"), core_from_string("00.0000") )
   );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("must stake a positive amount"),
                        stake( "alice1111111", core_from_string("0.0000"), core_from_string("00.0000") )

   );

   BOOST_REQUIRE_EQUAL( true, get_voter_info( "alice1111111" ).is_null() );
} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( unstake_negative, eosio_system_tester ) try {
   issue( "alice1111111", core_from_string("1000.0000"),  config::system_account_name );

   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "bob111111111", core_from_string("200.0001"), core_from_string("100.0001") ) );

   auto total = get_total_stake( "bob111111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("210.0001"), total["net_weight"].as<asset>());

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("must unstake a positive amount"),
                        unstake( "alice1111111", "bob111111111", core_from_string("-1.0000"), core_from_string("0.0000") )
   );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("must unstake a positive amount"),
                        unstake( "alice1111111", "bob111111111", core_from_string("0.0000"), core_from_string("-1.0000") )
   );

   //unstake all zeros
   BOOST_REQUIRE_EQUAL( wasm_assert_msg("must unstake a positive amount"),
                        unstake( "alice1111111", "bob111111111", core_from_string("0.0000"), core_from_string("0.0000") )

   );

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( unstake_more_than_at_stake, eosio_system_tester ) try {
   //cross_15_percent_threshold();

   issue( "alice1111111", core_from_string("1000.0000"),  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", core_from_string("200.0000"), core_from_string("100.0000") ) );

   auto total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["cpu_weight"].as<asset>());

   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );

   //trying to unstake more net bandwith than at stake

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("insufficient staked net bandwidth"),
                        unstake( "alice1111111", core_from_string("200.0001"), core_from_string("0.0000") )
   );

   //trying to unstake more cpu bandwith than at stake
   BOOST_REQUIRE_EQUAL( wasm_assert_msg("insufficient staked cpu bandwidth"),
                        unstake( "alice1111111", core_from_string("0.0000"), core_from_string("100.0001") )

   );

   //check that nothing has changed
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["cpu_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );
} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( delegate_to_another_user, eosio_system_tester ) try {
   //cross_15_percent_threshold();

   issue( "alice1111111", core_from_string("1000.0000"),  config::system_account_name );

   BOOST_REQUIRE_EQUAL( success(), stake ( "alice1111111", "bob111111111", core_from_string("200.0000"), core_from_string("100.0000") ) );

   auto total = get_total_stake( "bob111111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["cpu_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );

   //bob111111111 should not be able to unstake what was staked by alice1111111
   BOOST_REQUIRE_EQUAL( wasm_assert_msg("insufficient staked cpu bandwidth"),
                        unstake( "bob111111111", core_from_string("0.0000"), core_from_string("10.0000") )

   );
   BOOST_REQUIRE_EQUAL( wasm_assert_msg("insufficient staked net bandwidth"),
                        unstake( "bob111111111", core_from_string("10.0000"),  core_from_string("0.0000") )
   );

   issue( "carol1111111", core_from_string("1000.0000"),  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "carol1111111", "bob111111111", core_from_string("20.0000"), core_from_string("10.0000") ) );
   total = get_total_stake( "bob111111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("230.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("120.0000"), total["cpu_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("970.0000"), get_balance( "carol1111111" ) );

   //alice1111111 should not be able to unstake money staked by carol1111111

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("insufficient staked net bandwidth"),
                        unstake( "alice1111111", "bob111111111", core_from_string("2001.0000"), core_from_string("1.0000") )
   );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("insufficient staked cpu bandwidth"),
                        unstake( "alice1111111", "bob111111111", core_from_string("1.0000"), core_from_string("101.0000") )

   );

   total = get_total_stake( "bob111111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("230.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("120.0000"), total["cpu_weight"].as<asset>());
   //balance should not change after unsuccessfull attempts to unstake
   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( stake_unstake_separate, eosio_system_tester ) try {
   //cross_15_percent_threshold();

   issue( "alice1111111", core_from_string("1000.0000"),  config::system_account_name );
   BOOST_REQUIRE_EQUAL( core_from_string("1000.0000"), get_balance( "alice1111111" ) );

   //everything at once
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", core_from_string("10.0000"), core_from_string("20.0000") ) );
   auto total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("20.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("30.0000"), total["cpu_weight"].as<asset>());

   //cpu
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", core_from_string("100.0000"), core_from_string("0.0000") ) );
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("120.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("30.0000"), total["cpu_weight"].as<asset>());

   //net
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", core_from_string("0.0000"), core_from_string("200.0000") ) );
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("120.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("230.0000"), total["cpu_weight"].as<asset>());

   //unstake cpu
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice1111111", core_from_string("100.0000"), core_from_string("0.0000") ) );
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("20.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("230.0000"), total["cpu_weight"].as<asset>());

   //unstake net
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice1111111", core_from_string("0.0000"), core_from_string("200.0000") ) );
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("20.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("30.0000"), total["cpu_weight"].as<asset>());
} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( adding_stake_partial_unstake, eosio_system_tester ) try {
   //cross_15_percent_threshold();

   issue( "alice1111111", core_from_string("1000.0000"),  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "bob111111111", core_from_string("200.0000"), core_from_string("100.0000") ) );


   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "bob111111111", core_from_string("100.0000"), core_from_string("50.0000") ) );

   auto total = get_total_stake( "bob111111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("310.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("160.0000"), total["cpu_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("550.0000"), get_balance( "alice1111111" ) );

   //unstake a share
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice1111111", "bob111111111", core_from_string("150.0000"), core_from_string("75.0000") ) );

   total = get_total_stake( "bob111111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("160.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("85.0000"), total["cpu_weight"].as<asset>());

   //unstake more
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice1111111", "bob111111111", core_from_string("50.0000"), core_from_string("25.0000") ) );
   total = get_total_stake( "bob111111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("60.0000"), total["cpu_weight"].as<asset>());

   //combined amount should be available only in 3 days
   produce_block( fc::days(2) );
   produce_blocks(1);
   BOOST_REQUIRE_EQUAL( core_from_string("550.0000"), get_balance( "alice1111111" ) );
   produce_block( fc::days(1) );
   produce_blocks(1);
   BOOST_REQUIRE_EQUAL( core_from_string("850.0000"), get_balance( "alice1111111" ) );

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( stake_from_refund, eosio_system_tester ) try {
   //cross_15_percent_threshold();

   issue( "alice1111111", core_from_string("1000.0000"),  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "alice1111111", core_from_string("200.0000"), core_from_string("100.0000") ) );

   auto total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["cpu_weight"].as<asset>());

   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "bob111111111", core_from_string("50.0000"), core_from_string("50.0000") ) );

   total = get_total_stake( "bob111111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("60.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("60.0000"), total["cpu_weight"].as<asset>());

   BOOST_REQUIRE_EQUAL( core_from_string("600.0000"), get_balance( "alice1111111" ) );

   //unstake a share
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice1111111", "alice1111111", core_from_string("100.0000"), core_from_string("50.0000") ) );
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("60.0000"), total["cpu_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("600.0000"), get_balance( "alice1111111" ) );
   auto refund = get_refund_request( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("100.0000"), refund["net_amount"].as<asset>() );
   BOOST_REQUIRE_EQUAL( core_from_string( "50.0000"), refund["cpu_amount"].as<asset>() );
   //XXX auto request_time = refund["request_time"].as_int64();

   //alice delegates to bob, should pull from liquid balance not refund
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "bob111111111", core_from_string("50.0000"), core_from_string("50.0000") ) );
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("60.0000"), total["cpu_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("500.0000"), get_balance( "alice1111111" ) );
   refund = get_refund_request( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("100.0000"), refund["net_amount"].as<asset>() );
   BOOST_REQUIRE_EQUAL( core_from_string( "50.0000"), refund["cpu_amount"].as<asset>() );

   //stake less than pending refund, entire amount should be taken from refund
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "alice1111111", core_from_string("50.0000"), core_from_string("25.0000") ) );
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("160.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("85.0000"), total["cpu_weight"].as<asset>());
   refund = get_refund_request( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("50.0000"), refund["net_amount"].as<asset>() );
   BOOST_REQUIRE_EQUAL( core_from_string("25.0000"), refund["cpu_amount"].as<asset>() );
   //request time should stay the same
   //BOOST_REQUIRE_EQUAL( request_time, refund["request_time"].as_int64() );
   //balance should stay the same
   BOOST_REQUIRE_EQUAL( core_from_string("500.0000"), get_balance( "alice1111111" ) );

   //stake exactly pending refund amount
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "alice1111111", core_from_string("50.0000"), core_from_string("25.0000") ) );
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["cpu_weight"].as<asset>());
   //pending refund should be removed
   refund = get_refund_request( "alice1111111" );
   BOOST_TEST_REQUIRE( refund.is_null() );
   //balance should stay the same
   BOOST_REQUIRE_EQUAL( core_from_string("500.0000"), get_balance( "alice1111111" ) );

   //create pending refund again
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice1111111", "alice1111111", core_from_string("200.0000"), core_from_string("100.0000") ) );
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("10.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("10.0000"), total["cpu_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("500.0000"), get_balance( "alice1111111" ) );
   refund = get_refund_request( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("200.0000"), refund["net_amount"].as<asset>() );
   BOOST_REQUIRE_EQUAL( core_from_string("100.0000"), refund["cpu_amount"].as<asset>() );

   //stake more than pending refund
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "alice1111111", core_from_string("300.0000"), core_from_string("200.0000") ) );
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("310.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000"), total["cpu_weight"].as<asset>());
   refund = get_refund_request( "alice1111111" );
   BOOST_TEST_REQUIRE( refund.is_null() );
   //200 core tokens should be taken from alice's account
   BOOST_REQUIRE_EQUAL( core_from_string("300.0000"), get_balance( "alice1111111" ) );

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( stake_to_another_user_not_from_refund, eosio_system_tester ) try {
   //cross_15_percent_threshold();

   issue( "alice1111111", core_from_string("1000.0000"),  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", core_from_string("200.0000"), core_from_string("100.0000") ) );

   auto total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["cpu_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );

   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );

   //unstake
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice1111111", core_from_string("200.0000"), core_from_string("100.0000") ) );
   auto refund = get_refund_request( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("200.0000"), refund["net_amount"].as<asset>() );
   BOOST_REQUIRE_EQUAL( core_from_string("100.0000"), refund["cpu_amount"].as<asset>() );
   //auto orig_request_time = refund["request_time"].as_int64();

   //stake to another user
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "bob111111111", core_from_string("200.0000"), core_from_string("100.0000") ) );
   total = get_total_stake( "bob111111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["cpu_weight"].as<asset>());
   //stake should be taken from alices' balance, and refund request should stay the same
   BOOST_REQUIRE_EQUAL( core_from_string("400.0000"), get_balance( "alice1111111" ) );
   refund = get_refund_request( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("200.0000"), refund["net_amount"].as<asset>() );
   BOOST_REQUIRE_EQUAL( core_from_string("100.0000"), refund["cpu_amount"].as<asset>() );
   //BOOST_REQUIRE_EQUAL( orig_request_time, refund["request_time"].as_int64() );

} FC_LOG_AND_RETHROW()

// Tests for voting
BOOST_FIXTURE_TEST_CASE( producer_register_unregister, eosio_system_tester ) try {
   issue( "alice1111111", core_from_string("1000.0000"),  config::system_account_name );

   //fc::variant params = producer_parameters_example(1);
   auto key =  fc::crypto::public_key( std::string("EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV") );

   BOOST_REQUIRE_EQUAL( success(),push_action(N(eosio), N(addproducer), mvo()("producer",  "alice1111111")));
   BOOST_REQUIRE_EQUAL( success(), push_action(N(alice1111111), N(regproducer), mvo()
                                               ("producer",  "alice1111111")
                                               ("producer_key", key )
                                               ("url", "http://block.one")
                                               ("location", 1)
                        )
   );

   auto info = get_producer_info( "alice1111111" );
   BOOST_REQUIRE_EQUAL( "alice1111111", info["owner"].as_string() );
   BOOST_REQUIRE_EQUAL( "http://block.one", info["url"].as_string() );

   //change parameters one by one to check for things like #3783
   //fc::variant params2 = producer_parameters_example(2);
   BOOST_REQUIRE_EQUAL( success(), push_action(N(alice1111111), N(regproducer), mvo()
                                               ("producer",  "alice1111111")
                                               ("producer_key", key )
                                               ("url", "http://block.two")
                                               ("location", 1)
                        )
   );
   info = get_producer_info( "alice1111111" );
   BOOST_REQUIRE_EQUAL( "alice1111111", info["owner"].as_string() );
   BOOST_REQUIRE_EQUAL( key, fc::crypto::public_key(info["producer_key"].as_string()) );
   BOOST_REQUIRE_EQUAL( "http://block.two", info["url"].as_string() );
   BOOST_REQUIRE_EQUAL( 1, info["location"].as_int64() );

   auto key2 =  fc::crypto::public_key( std::string("EOS5jnmSKrzdBHE9n8hw58y7yxFWBC8SNiG7m8S1crJH3KvAnf9o6") );
   BOOST_REQUIRE_EQUAL( success(), push_action(N(alice1111111), N(regproducer), mvo()
                                               ("producer",  "alice1111111")
                                               ("producer_key", key2 )
                                               ("url", "http://block.two")
                                               ("location", 2)
                        )
   );
   info = get_producer_info( "alice1111111" );
   BOOST_REQUIRE_EQUAL( "alice1111111", info["owner"].as_string() );
   BOOST_REQUIRE_EQUAL( key2, fc::crypto::public_key(info["producer_key"].as_string()) );
   BOOST_REQUIRE_EQUAL( "http://block.two", info["url"].as_string() );
   BOOST_REQUIRE_EQUAL( 2, info["location"].as_int64() );

   //unregister producer
   BOOST_REQUIRE_EQUAL( success(), push_action(N(eosio), N(unregprod), mvo()
                                               ("producer",  "alice1111111")
                        )
   );
   info = get_producer_info( "alice1111111" );
   //key should be empty
   BOOST_REQUIRE_EQUAL( fc::crypto::public_key(), fc::crypto::public_key(info["producer_key"].as_string()) );
   //everything else should stay the same
   BOOST_REQUIRE_EQUAL( "alice1111111", info["owner"].as_string() );
   BOOST_REQUIRE_EQUAL( "http://block.two", info["url"].as_string() );

   //unregister bob111111111 who is not a producer
   BOOST_REQUIRE_EQUAL( wasm_assert_msg( "producer not found" ),
                        push_action( N(eosio), N(unregprod), mvo()
                                     ("producer",  "bob111111111")
                        )
   );

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(producer_pay, eosio_system_tester, * boost::unit_test::tolerance(1e-10)) try {
   activate_chain();

   const double continuous_rate = 5.827323 / 100.;
   const double usecs_per_year  = 52 * 7 * 24 * 3600 * 1000000ll;
   const double secs_per_year   = 52 * 7 * 24 * 3600;
   const uint64_t useconds_per_min      = 60 * uint64_t(1000000);
   const uint64_t useconds_per_year     = secs_per_year*1000000ll;

   const asset large_asset = core_from_string("80.0000");
   create_account_with_resources( N(defproducera), config::system_account_name, core_from_string("1000.0000"), false, large_asset, large_asset );
   create_account_with_resources( N(defproducerb), config::system_account_name, core_from_string("1000.0000"), false, large_asset, large_asset );
   create_account_with_resources( N(defproducerc), config::system_account_name, core_from_string("1000.0000"), false, large_asset, large_asset );

   create_account_with_resources( N(producvotera), config::system_account_name, core_from_string("1000.0000"), false, large_asset, large_asset );
   create_account_with_resources( N(producvoterb), config::system_account_name, core_from_string("1000.0000"), false, large_asset, large_asset );

   BOOST_REQUIRE_EQUAL(success(), regproducer(N(defproducera)));
   auto prod = get_producer_info( N(defproducera) );
   BOOST_REQUIRE_EQUAL("defproducera", prod["owner"].as_string());

   transfer( config::system_account_name, "producvotera", core_from_string("400000000.0000"), config::system_account_name);
   BOOST_REQUIRE_EQUAL(success(), stake("producvotera", core_from_string("100000000.0000"), core_from_string("100000000.0000")));

   {
      const asset initial_supply  = get_token_supply();

      // produce for 23 hours and 55 minutes 
      produce_block(fc::minutes(60 * 23 + 55 ));
      produce_blocks(1);

      const asset supply  = get_token_supply();

      const auto     global_state      = get_global_state();
      const int64_t  savings           = get_balance(N(eosio.saving)).get_amount();
      const int64_t  ppay           = get_balance(N(eosio.ppay)).get_amount();
      const int64_t  usage           = get_balance(N(eosio.usage)).get_amount();

      BOOST_REQUIRE_EQUAL(0, savings);
      BOOST_REQUIRE_EQUAL(0, ppay);
      BOOST_REQUIRE_EQUAL(0, usage);
      BOOST_REQUIRE_EQUAL(initial_supply.get_amount(), supply.get_amount());

   }


   {
      BOOST_REQUIRE_EQUAL(wasm_assert_msg("producer pay request not found"),
                          push_action(N(defproducera), N(claimrewards), mvo()("owner", "defproducera")));
   }



   {
      const auto     initial_global_state      = get_global_state();
      const uint64_t initial_distribution_time = initial_global_state["last_inflation_distribution"].as_uint64();
      const int64_t  initial_savings           = get_balance(N(eosio.saving)).get_amount();
      const int64_t  initial_usage             = get_balance(N(eosio.usage)).get_amount();
      const int64_t  initial_ppay              = get_balance(N(eosio.ppay)).get_amount();

      const asset initial_supply  = get_token_supply();

      // produce for 5 more minutes for first inflation distribution and defproducera can now claim their first rewards
      produce_block(fc::seconds(5 * 60));
      produce_blocks(1);

      const auto global_state          = get_global_state();
      const uint64_t distribution_time = global_state["last_inflation_distribution"].as_uint64();
      const int64_t  saving            = get_balance(N(eosio.saving)).get_amount();
      const int64_t  usage             = get_balance(N(eosio.usage)).get_amount();
      const int64_t  ppay              = get_balance(N(eosio.ppay)).get_amount();

      const asset supply  = get_token_supply();

      auto usecs_between_distributions = distribution_time - initial_distribution_time;
      int32_t secs_between_distributions = usecs_between_distributions/1000000;

      auto inflation = int64_t( ( initial_supply.get_amount() * double(usecs_between_distributions) * continuous_rate ) / useconds_per_year );

      prod = get_producer_info("defproducera");
      BOOST_REQUIRE_EQUAL(0, prod["last_claim_time"].as<uint64_t>());
      BOOST_REQUIRE_EQUAL(inflation, supply.get_amount() - initial_supply.get_amount());
      BOOST_REQUIRE_EQUAL(inflation, saving + usage + ppay);

      const uint16_t base_producer_rate = 250;
      auto max_to_producers   = inflation / 6;
      auto to_producers       = (max_to_producers * base_producer_rate) / 1000;

      int64_t to_savings          = max_to_producers + max_to_producers - to_producers;
      int64_t to_usage            = inflation - to_producers - to_savings;

      BOOST_REQUIRE_EQUAL(to_producers, ppay);
      BOOST_REQUIRE_EQUAL(to_savings, saving);
      BOOST_REQUIRE_EQUAL(to_usage, usage);
      BOOST_REQUIRE(to_usage > 2 * (to_producers + to_savings));
      
      BOOST_REQUIRE_EQUAL(success(), push_action(N(defproducera), N(claimrewards), mvo()("owner", "defproducera")));
      
      const asset balance = get_balance(N(defproducera));
      BOOST_REQUIRE_EQUAL(balance.get_amount(), to_producers);
      BOOST_REQUIRE_EQUAL(0,get_balance(N(eosio.ppay)).get_amount());


   }

   // defproducerb tries to claim rewards but he's not on the list
   {
      BOOST_REQUIRE_EQUAL(wasm_assert_msg("unable to find key"),
                          push_action(N(defproducerb), N(claimrewards), mvo()("owner", "defproducerb")));
   }


   // test change in network usage level to 5%
   {
      const auto     initial_global_state      = get_global_state();
      const uint64_t initial_distribution_time = initial_global_state["last_inflation_distribution"].as_uint64();
      const int64_t  initial_savings           = get_balance(N(eosio.saving)).get_amount();
      const int64_t  initial_usage             = get_balance(N(eosio.usage)).get_amount();
      const int64_t  initial_ppay              = get_balance(N(eosio.ppay)).get_amount();

      const asset initial_balance = get_balance(N(defproducera));

      const asset initial_supply  = get_token_supply();

      set_network_usage(5);

      // produce for another day
      produce_block(fc::minutes(60 * 24 + 10));
      produce_blocks(1);

      const auto global_state          = get_global_state();
      const uint64_t distribution_time = global_state["last_inflation_distribution"].as_uint64();
      const int64_t  saving            = get_balance(N(eosio.saving)).get_amount();
      const int64_t  usage             = get_balance(N(eosio.usage)).get_amount();
      const int64_t  ppay              = get_balance(N(eosio.ppay)).get_amount();

      const asset supply  = get_token_supply();

      auto usecs_between_distributions = distribution_time - initial_distribution_time;
      int32_t secs_between_distributions = usecs_between_distributions/1000000;

      auto inflation = int64_t( ( initial_supply.get_amount() * double(usecs_between_distributions) * continuous_rate ) / useconds_per_year );

      BOOST_REQUIRE_EQUAL(inflation, supply.get_amount() - initial_supply.get_amount());
      BOOST_REQUIRE_EQUAL(inflation, saving - initial_savings + usage - initial_usage + ppay);

      const uint16_t producer_rate = 250 + 25 * 5;
      auto max_to_producers   = inflation / 6;
      auto to_producers       = (max_to_producers * producer_rate) / 1000;

      int64_t to_savings          = max_to_producers + max_to_producers - to_producers;
      int64_t to_usage            = inflation - to_producers - to_savings;

      BOOST_REQUIRE_EQUAL(to_producers, ppay);
      BOOST_REQUIRE_EQUAL(to_savings, saving - initial_savings);
      BOOST_REQUIRE_EQUAL(to_usage, usage - initial_usage);
      BOOST_REQUIRE(to_usage > 2 * (to_producers + to_savings));
      
      BOOST_REQUIRE_EQUAL(success(), push_action(N(defproducera), N(claimrewards), mvo()("owner", "defproducera")));
      
      const asset balance = get_balance(N(defproducera));
      BOOST_REQUIRE_EQUAL(balance.get_amount() - initial_balance.get_amount(), to_producers);
      BOOST_REQUIRE_EQUAL(0,get_balance(N(eosio.ppay)).get_amount());

   }   

   // test change in network usage level to 30%
   {
      const auto     initial_global_state      = get_global_state();
      const uint64_t initial_distribution_time = initial_global_state["last_inflation_distribution"].as_uint64();
      const int64_t  initial_savings           = get_balance(N(eosio.saving)).get_amount();
      const int64_t  initial_usage             = get_balance(N(eosio.usage)).get_amount();
      const int64_t  initial_ppay              = get_balance(N(eosio.ppay)).get_amount();

      const asset initial_balance = get_balance(N(defproducera));

      const asset initial_supply  = get_token_supply();

      set_network_usage(31);

      // produce for another day
      produce_block(fc::minutes(60 * 24 + 10));
      produce_blocks(1);

      const auto global_state          = get_global_state();
      const uint64_t distribution_time = global_state["last_inflation_distribution"].as_uint64();
      const int64_t  saving            = get_balance(N(eosio.saving)).get_amount();
      const int64_t  usage             = get_balance(N(eosio.usage)).get_amount();
      const int64_t  ppay              = get_balance(N(eosio.ppay)).get_amount();

      const asset supply  = get_token_supply();

      auto usecs_between_distributions = distribution_time - initial_distribution_time;
      int32_t secs_between_distributions = usecs_between_distributions/1000000;

      auto inflation = int64_t( ( initial_supply.get_amount() * double(usecs_between_distributions) * continuous_rate ) / useconds_per_year );

      BOOST_REQUIRE_EQUAL(inflation, supply.get_amount() - initial_supply.get_amount());
      BOOST_REQUIRE_EQUAL(inflation, saving - initial_savings + usage - initial_usage + ppay);

      auto to_producers           = inflation / 6;
      int64_t to_savings          = to_producers;
      int64_t to_usage            = inflation - to_producers - to_savings;

      BOOST_REQUIRE_EQUAL(to_producers, ppay - initial_ppay);
      BOOST_REQUIRE_EQUAL(to_savings, saving - initial_savings);
      BOOST_REQUIRE_EQUAL(to_usage, usage - initial_usage);
      BOOST_REQUIRE(to_usage >= 2 * (to_producers + to_savings));

      
      BOOST_REQUIRE_EQUAL(success(), push_action(N(defproducera), N(claimrewards), mvo()("owner", "defproducera")));
      
      const asset balance = get_balance(N(defproducera));
      BOOST_REQUIRE_EQUAL(balance.get_amount() - initial_balance.get_amount(), to_producers);
      BOOST_REQUIRE_EQUAL(0,get_balance(N(eosio.ppay)).get_amount());

   } 

   // test for 7 years
   {
      produce_block(fc::days(2555)); 
      produce_blocks(1);
      const asset   supply  = get_token_supply();
      std::cout << "###############supply " << supply.get_amount() << "\n";

   }

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(inflation_year_stablility, eosio_system_tester, * boost::unit_test::tolerance(1e-10)) try {
   activate_chain();

   const double continuous_rate = 5.82729 / 100.;
   const double usecs_per_year  = 52 * 7 * 24 * 3600 * 1000000ll;
   const double secs_per_year   = 52 * 7 * 24 * 3600;
   const uint64_t useconds_per_min      = 60 * uint64_t(1000000);
   const uint64_t useconds_per_year     = secs_per_year*1000000ll;

   const asset large_asset = core_from_string("80.0000");
   create_account_with_resources( N(defproducera), config::system_account_name, core_from_string("1000.0000"), false, large_asset, large_asset );
   create_account_with_resources( N(defproducerb), config::system_account_name, core_from_string("1000.0000"), false, large_asset, large_asset );
   create_account_with_resources( N(defproducerc), config::system_account_name, core_from_string("1000.0000"), false, large_asset, large_asset );


   BOOST_REQUIRE_EQUAL(success(), regproducer(N(defproducera)));
   regproducer(N(defproducerb));
   regproducer(N(defproducerc));
   const asset   initial_supply  = get_token_supply();
   const int64_t initial_savings = get_balance(N(eosio.saving)).get_amount();
   const int64_t initial_usage = get_balance(N(eosio.usage)).get_amount();

   for (uint32_t i = 0; i < 7 * 52; ++i) {
      produce_block(fc::seconds(8 * 3600));
      produce_block(fc::seconds(8 * 3600));
      produce_block(fc::seconds(8 * 3600));
      produce_blocks(1);
   }

   BOOST_REQUIRE_EQUAL(success(), push_action(N(defproducerc), N(claimrewards), mvo()("owner", "defproducerc")));
   BOOST_REQUIRE_EQUAL(success(), push_action(N(defproducerb), N(claimrewards), mvo()("owner", "defproducerb")));
   BOOST_REQUIRE_EQUAL(success(), push_action(N(defproducera), N(claimrewards), mvo()("owner", "defproducera")));

   const asset   supply  = get_token_supply();
   const int64_t savings = get_balance(N(eosio.saving)).get_amount();
   const int64_t usage = get_balance(N(eosio.usage)).get_amount();
   
   // Amount issued per year is very close to the 6% inflation target. Small difference (50 tokens out of 60,000,000 issued)
   // is due to compounding every 24 hours as opposed to theoretical continuous compounding
   BOOST_REQUIRE(0 < int64_t(double(initial_supply.get_amount()) * double(0.06)) - (supply.get_amount() - initial_supply.get_amount()));
   BOOST_REQUIRE(50 * 10000 > int64_t(double(initial_supply.get_amount()) * double(0.06)) - (supply.get_amount() - initial_supply.get_amount()));
   BOOST_REQUIRE(0 < int64_t(double(initial_supply.get_amount()) * double(0.0175)) - (savings - initial_savings));
   BOOST_REQUIRE(50 * 10000 > int64_t(double(initial_supply.get_amount()) * double(0.0175)) - (savings - initial_savings));
   BOOST_REQUIRE(0 < int64_t(double(initial_supply.get_amount()) * double(0.04)) - (usage - initial_usage));
   BOOST_REQUIRE(50 * 10000 > int64_t(double(initial_supply.get_amount()) * double(0.04)) - (usage - initial_usage));

} FC_LOG_AND_RETHROW()

// todo test produced blocks

BOOST_FIXTURE_TEST_CASE(multiple_producer_pay, eosio_system_tester, * boost::unit_test::tolerance(1e-10)) try {

   auto within_one = [](int64_t a, int64_t b) -> bool { return std::abs( a - b ) <= 1; };

   const int64_t secs_per_year  = 52 * 7 * 24 * 3600;
   const double  usecs_per_year = secs_per_year * 1000000;
   const double  cont_rate      = 5.827323 / 100.;

   const asset net = core_from_string("80.0000");
   const asset cpu = core_from_string("80.0000");

   // create accounts {defproducera, defproducerb, ..., defproducerz, abcproducera, ..., defproducern} and register as producers
   std::vector<account_name> producer_names;
   {
      producer_names.reserve('z' - 'a' + 1);
      {
         const std::string root("defproducer");
         for ( char c = 'a'; c <= 'z'; ++c ) {
            producer_names.emplace_back(root + std::string(1, c));
         }
      }

      activate_chain();

      setup_producer_accounts(producer_names);
      for (const auto& p: producer_names) {
         BOOST_REQUIRE_EQUAL( success(), regproducer(p) );
         //produce_blocks(1);
         ilog( "------ get pro----------" );
         wdump((p));
      }
   }


   {
      auto proda = get_producer_info( N(defproducera) );
      auto prodj = get_producer_info( N(defproducerj) );
      auto prodk = get_producer_info( N(defproducerk) );
      auto produ = get_producer_info( N(defproduceru) );
      auto prodv = get_producer_info( N(defproducerv) );
      auto prodz = get_producer_info( N(defproducerz) );

      BOOST_REQUIRE (0 == proda["produced_blocks"].as<uint32_t>() && 0 == prodz["produced_blocks"].as<uint32_t>());
      BOOST_REQUIRE (0 == proda["last_claim_time"].as<uint64_t>() && 0 == prodz["last_claim_time"].as<uint64_t>());

   }

   {
      const uint32_t prod_index = 0;
      const auto prod_name = producer_names[prod_index];

      const auto     initial_global_state      = get_global_state();
      const uint64_t initial_distribution_time = initial_global_state["last_inflation_distribution"].as_uint64();
      const int64_t  initial_savings           = get_balance(N(eosio.saving)).get_amount();
      const asset    initial_supply            = get_token_supply();
      const asset    initial_usage             = get_balance(N(eosio.usage));
      const asset    initial_balance           = get_balance(prod_name);

      // Wait for 24 hours for the first ditribution. By now, pervote_bucket has grown enough

      produce_block(fc::minutes(20*60 * 24 + 1));
      produce_blocks(1);

      // BOOST_REQUIRE_EQUAL(success(), push_action(prod_name, N(claimrewards), mvo()("owner", prod_name)));

      const auto     global_state      = get_global_state();
      const uint64_t distribution_time = global_state["last_inflation_distribution"].as_uint64();
      const int64_t  saving            = get_balance(N(eosio.saving)).get_amount();
      const int64_t  usage             = get_balance(N(eosio.usage)).get_amount();
      const int64_t  initial_ppay      = get_balance(N(eosio.ppay)).get_amount();
      const asset    balance           = get_balance(prod_name);

      const uint64_t usecs_between_fills = distribution_time - initial_distribution_time;
      const int32_t secs_between_fills = static_cast<int32_t>(usecs_between_fills / 1000000);

      const int64_t expected_supply_growth = static_cast<int64_t>(initial_supply.get_amount() * double(usecs_between_fills) * cont_rate / usecs_per_year);

      BOOST_REQUIRE_EQUAL(expected_supply_growth, saving + usage + initial_ppay);

      auto producer_pay = initial_ppay / producer_names.size();      
      BOOST_REQUIRE_EQUAL(success(), push_action(prod_name, N(claimrewards), mvo()("owner", prod_name)));
      BOOST_REQUIRE_EQUAL(wasm_assert_msg("producer pay request not found"),
                        push_action(prod_name, N(claimrewards), mvo()("owner", prod_name)));
      
      const int64_t  ppay              = get_balance(N(eosio.ppay)).get_amount();
      BOOST_REQUIRE_EQUAL(ppay, initial_ppay - producer_pay);

      for ( size_t i = 1; i < producer_names.size(); ++i ) {
         BOOST_REQUIRE_EQUAL(success(), push_action(producer_names[i], N(claimrewards), mvo()("owner", producer_names[i])));
         const asset balance = get_balance(producer_names[i]);
         BOOST_REQUIRE_EQUAL( balance.get_amount(), producer_pay );         
      }

      // change due to rounding errors left in account after all producers collect
      BOOST_REQUIRE( get_balance(N(eosio.ppay)).get_amount() < producer_names.size());  
      
   }


   {
      const uint32_t rmv_index = 5;
      account_name prod_name = producer_names[rmv_index];

      auto info = get_producer_info(prod_name);
      BOOST_REQUIRE( info["is_active"].as<bool>() );
      BOOST_REQUIRE( fc::crypto::public_key() != fc::crypto::public_key(info["producer_key"].as_string()) );

      BOOST_REQUIRE_EQUAL( error("missing authority of eosio"),
                           push_action(prod_name, N(rmvproducer), mvo()("producer", prod_name)));
      BOOST_REQUIRE_EQUAL( error("missing authority of eosio"),
                           push_action(producer_names[rmv_index + 2], N(rmvproducer), mvo()("producer", prod_name) ) );
      BOOST_REQUIRE_EQUAL( success(),
                           push_action(config::system_account_name, N(rmvproducer), mvo()("producer", prod_name) ) );

      produce_blocks(3 * 21 * 12);
      BOOST_REQUIRE_EQUAL( true, get_producer_info(prod_name).is_null() );
      //info = get_producer_info(prod_name);
      //BOOST_REQUIRE( !info["is_active"].as<bool>() );
      //BOOST_REQUIRE( fc::crypto::public_key() == fc::crypto::public_key(info["producer_key"].as_string()) );
      BOOST_REQUIRE_EQUAL( wasm_assert_msg("unable to find key"),
                           push_action(prod_name, N(claimrewards), mvo()("owner", prod_name) ) );


   }

   {
      BOOST_REQUIRE_EQUAL( wasm_assert_msg("producer not found"),
                           push_action( config::system_account_name, N(rmvproducer), mvo()("producer", "nonexistingp") ) );
   }

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(producers_upgrade_system_contract, eosio_system_tester) try {
   //install multisig contract
   abi_serializer msig_abi_ser = initialize_multisig();
   auto producer_names = active_and_vote_producers();

   //helper function
   auto push_action_msig = [&]( const account_name& signer, const action_name &name, const variant_object &data, bool auth = true ) -> action_result {
         string action_type_name = msig_abi_ser.get_action_type(name);

         action act;
         act.account = N(eosio.msig);
         act.name = name;
         act.data = msig_abi_ser.variant_to_binary( action_type_name, data, abi_serializer_max_time );

         return base_tester::push_action( std::move(act), auth ? uint64_t(signer) : signer == N(bob111111111) ? N(alice1111111) : N(bob111111111) );
   };
   // test begins
   vector<permission_level> prod_perms;
   for ( auto& x : producer_names ) {
      prod_perms.push_back( { name(x), config::active_name } );
   }
   //prepare system contract with different hash (contract differs in one byte)
   string eosio_system_wast2 = eosio_system_wast;
   string msg = "account is not registered as a producer";
   auto pos = eosio_system_wast2.find(msg);
   BOOST_REQUIRE( pos != std::string::npos );
   msg[0] = 'A';
   eosio_system_wast2.replace( pos, msg.size(), msg );

   transaction trx;
   {
      auto code = wast_to_wasm( eosio_system_wast2 );
      variant pretty_trx = fc::mutable_variant_object()
         ("expiration", "2020-01-01T00:30")
         ("ref_block_num", 2)
         ("ref_block_prefix", 3)
         ("max_net_usage_words", 0)
         ("max_cpu_usage_ms", 0)
         ("delay_sec", 0)
         ("actions", fc::variants({
               fc::mutable_variant_object()
                  ("account", name(config::system_account_name))
                  ("name", "setcode")
                  ("authorization", vector<permission_level>{ { config::system_account_name, config::active_name } })
                  ("data", fc::mutable_variant_object() ("account", name(config::system_account_name))
                   ("vmtype", 0)
                   ("vmversion", "0")
                   ("code", bytes( code.begin(), code.end() ))
                  )
                  })
         );
      abi_serializer::from_variant(pretty_trx, trx, get_resolver(), abi_serializer_max_time);
   }

   BOOST_REQUIRE_EQUAL(success(), push_action_msig( N(alice1111111), N(propose), mvo()
                                                    ("proposer",      "alice1111111")
                                                    ("proposal_name", "upgrade1")
                                                    ("trx",           trx)
                                                    ("requested", prod_perms)
                       )
   );

   // get 15 approvals
   for ( size_t i = 0; i < 14; ++i ) {
      BOOST_REQUIRE_EQUAL(success(), push_action_msig( name(producer_names[i]), N(approve), mvo()
                                                       ("proposer",      "alice1111111")
                                                       ("proposal_name", "upgrade1")
                                                       ("level",         permission_level{ name(producer_names[i]), config::active_name })
                          )
      );
   }

   //should fail
   BOOST_REQUIRE_EQUAL(wasm_assert_msg("transaction authorization failed"),
                       push_action_msig( N(alice1111111), N(exec), mvo()
                                         ("proposer",      "alice1111111")
                                         ("proposal_name", "upgrade1")
                                         ("executer",      "alice1111111")
                       )
   );

   // one more approval
   BOOST_REQUIRE_EQUAL(success(), push_action_msig( name(producer_names[14]), N(approve), mvo()
                                                    ("proposer",      "alice1111111")
                                                    ("proposal_name", "upgrade1")
                                                    ("level",         permission_level{ name(producer_names[14]), config::active_name })
                          )
   );

   transaction_trace_ptr trace;
   control->applied_transaction.connect([&]( const transaction_trace_ptr& t) { if (t->scheduled) { trace = t; } } );
   BOOST_REQUIRE_EQUAL(success(), push_action_msig( N(alice1111111), N(exec), mvo()
                                                    ("proposer",      "alice1111111")
                                                    ("proposal_name", "upgrade1")
                                                    ("executer",      "alice1111111")
                       )
   );

   BOOST_REQUIRE( bool(trace) );
   BOOST_REQUIRE_EQUAL( 1, trace->action_traces.size() );
   BOOST_REQUIRE_EQUAL( transaction_receipt::executed, trace->receipt->status );

   produce_blocks( 250 );

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE(producer_onblock_check, eosio_system_tester) try {

   // create accounts {defproducera, defproducerb, ..., defproducerz} and register as producers
   std::vector<account_name> producer_names;
   producer_names.reserve('z' - 'a' + 1);
   const std::string root("defproducer");
   for ( char c = 'a'; c <= 'z'; ++c ) {
      producer_names.emplace_back(root + std::string(1, c));
   }
   setup_producer_accounts(producer_names);

   for (auto a:producer_names)
      BOOST_REQUIRE_EQUAL( success(), regproducer(a) );



   // produce for 1 hour
   {
      produce_block(fc::seconds(3600));
      bool all_21_produced = true;
      for (uint32_t i = 0; i < 21; ++i) {
         if (0 == get_producer_info(producer_names[i])["produced_blocks"].as<uint32_t>()) {
            all_21_produced= false;
         }
      }
      BOOST_REQUIRE_EQUAL(false, all_21_produced);

      const auto     global_state      = get_global_state();
      const int64_t  saving            = get_balance(N(eosio.saving)).get_amount();
      const int64_t  usage             = get_balance(N(eosio.usage)).get_amount();
      const int64_t  ppay              = get_balance(N(eosio.ppay)).get_amount();

      BOOST_REQUIRE_EQUAL(0, saving);
      BOOST_REQUIRE_EQUAL(0, usage);
      BOOST_REQUIRE_EQUAL(0, ppay);

      BOOST_REQUIRE_EQUAL(wasm_assert_msg( "producer pay request not found" ),
                          push_action(producer_names.front(), N(claimrewards), mvo()("owner", producer_names.front())));

   }

   {

      activate_chain();
      // need to produce blocks otherwise time based produce_block doesn;t work
      produce_blocks(5);

      // produce for 24 hours
      produce_block(fc::seconds(24 * 3600));

      auto info = get_producer_info(producer_names.front());

      BOOST_REQUIRE_EQUAL(success(),
                          push_action(producer_names.front(), N(claimrewards), mvo()("owner", producer_names.front())));
      BOOST_REQUIRE_EQUAL(success(),
                          push_action(producer_names.back(), N(claimrewards), mvo()("owner", producer_names.back())));
      BOOST_REQUIRE_EQUAL(get_balance(producer_names.front()).get_amount(), get_balance(producer_names.back()).get_amount());

   }


} FC_LOG_AND_RETHROW()


fc::mutable_variant_object config_to_variant( const eosio::chain::chain_config& config ) {
   return mutable_variant_object()
      ( "max_block_net_usage", config.max_block_net_usage )
      ( "target_block_net_usage_pct", config.target_block_net_usage_pct )
      ( "max_transaction_net_usage", config.max_transaction_net_usage )
      ( "base_per_transaction_net_usage", config.base_per_transaction_net_usage )
      ( "context_free_discount_net_usage_num", config.context_free_discount_net_usage_num )
      ( "context_free_discount_net_usage_den", config.context_free_discount_net_usage_den )
      ( "max_block_cpu_usage", config.max_block_cpu_usage )
      ( "target_block_cpu_usage_pct", config.target_block_cpu_usage_pct )
      ( "max_transaction_cpu_usage", config.max_transaction_cpu_usage )
      ( "min_transaction_cpu_usage", config.min_transaction_cpu_usage )
      ( "max_transaction_lifetime", config.max_transaction_lifetime )
      ( "deferred_trx_expiration_window", config.deferred_trx_expiration_window )
      ( "max_transaction_delay", config.max_transaction_delay )
      ( "max_inline_action_size", config.max_inline_action_size )
      ( "max_inline_action_depth", config.max_inline_action_depth )
      ( "max_authority_depth", config.max_authority_depth );
}

BOOST_FIXTURE_TEST_CASE( elect_producers /*_and_parameters*/, eosio_system_tester ) try {
   create_accounts_with_resources( {  N(defproducer1), N(defproducer2), N(defproducer3) } );
   BOOST_REQUIRE_EQUAL( success(), regproducer( "defproducer1", 1) );
   BOOST_REQUIRE_EQUAL( success(), regproducer( "defproducer2", 2) );
   BOOST_REQUIRE_EQUAL( success(), regproducer( "defproducer3", 3) );

   //stake more than 15% of total EOS supply to activate chain
   transfer( "eosio", "alice1111111", core_from_string("600000000.0000"), "eosio" );
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "alice1111111", core_from_string("300000000.0000"), core_from_string("300000000.0000") ) );
   //vote for producers
   activate_chain();
   produce_blocks(250);
   auto producer_keys = control->head_block_state()->active_schedule.producers;
   BOOST_REQUIRE_EQUAL( 3, producer_keys.size() );
   BOOST_REQUIRE_EQUAL( name("defproducer1"), producer_keys[0].producer_name );
   BOOST_REQUIRE_EQUAL( name("defproducer2"), producer_keys[1].producer_name );
   BOOST_REQUIRE_EQUAL( name("defproducer3"), producer_keys[2].producer_name );

   // TODO test some more schedule changes

} FC_LOG_AND_RETHROW()

/**
BOOST_FIXTURE_TEST_CASE( buyname, eosio_system_tester ) try {
   create_accounts_with_resources( { N(dan), N(sam) } );
   transfer( config::system_account_name, "dan", core_from_string( "10000.0000" ) );
   transfer( config::system_account_name, "sam", core_from_string( "10000.0000" ) );
   stake_with_transfer( config::system_account_name, "sam", core_from_string( "80000000.0000" ), core_from_string( "80000000.0000" ) );
   stake_with_transfer( config::system_account_name, "dan", core_from_string( "80000000.0000" ), core_from_string( "80000000.0000" ) );

   regproducer( config::system_account_name );
   // wait 14 days after min required amount has been staked
   produce_block( fc::days(7) );
   produce_block( fc::days(7) );
   produce_block();

   BOOST_REQUIRE_EXCEPTION( create_accounts_with_resources( { N(fail) }, N(dan) ), // dan shouldn't be able to create fail
                            eosio_assert_message_exception, eosio_assert_message_is( "no active bid for name" ) );
   bidname( "dan", "nofail", core_from_string( "1.0000" ) );
   BOOST_REQUIRE_EQUAL( "assertion failure with message: must increase bid by 10%", bidname( "sam", "nofail", core_from_string( "1.0000" ) )); // didn't increase bid by 10%
   BOOST_REQUIRE_EQUAL( success(), bidname( "sam", "nofail", core_from_string( "2.0000" ) )); // didn't increase bid by 10%
   produce_block( fc::days(1) );
   produce_block();

   BOOST_REQUIRE_EXCEPTION( create_accounts_with_resources( { N(nofail) }, N(dan) ), // dan shoudn't be able to do this, sam won
                            eosio_assert_message_exception, eosio_assert_message_is( "only highest bidder can claim" ) );
   //wlog( "verify sam can create nofail" );
   create_accounts_with_resources( { N(nofail) }, N(sam) ); // sam should be able to do this, he won the bid
   //wlog( "verify nofail can create test.nofail" );
   transfer( "eosio", "nofail", core_from_string( "1000.0000" ) );
   create_accounts_with_resources( { N(test.nofail) }, N(nofail) ); // only nofail can create test.nofail
   //wlog( "verify dan cannot create test.fail" );
   BOOST_REQUIRE_EXCEPTION( create_accounts_with_resources( { N(test.fail) }, N(dan) ), // dan shouldn't be able to do this
                            eosio_assert_message_exception, eosio_assert_message_is( "only suffix may create this account" ) );

   create_accounts_with_resources( { N(goodgoodgood) }, N(dan) ); /// 12 char names should succeed
} FC_LOG_AND_RETHROW()
**/

/**
BOOST_FIXTURE_TEST_CASE( bid_invalid_names, eosio_system_tester ) try {
   create_accounts_with_resources( { N(dan) } );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg( "you can only bid on top-level suffix" ),
                        bidname( "dan", "abcdefg.12345", core_from_string( "1.0000" ) ) );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg( "the empty name is not a valid account name to bid on" ),
                        bidname( "dan", "", core_from_string( "1.0000" ) ) );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg( "13 character names are not valid account names to bid on" ),
                        bidname( "dan", "abcdefgh12345", core_from_string( "1.0000" ) ) );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg( "accounts with 12 character names and no dots can be created without bidding required" ),
                        bidname( "dan", "abcdefg12345", core_from_string( "1.0000" ) ) );

} FC_LOG_AND_RETHROW()
**/

/**
BOOST_FIXTURE_TEST_CASE( multiple_namebids, eosio_system_tester ) try {

   const std::string not_closed_message("auction for name is not closed yet");

   std::vector<account_name> accounts = { N(alice), N(bob), N(carl), N(david), N(eve) };
   create_accounts_with_resources( accounts );
   for ( const auto& a: accounts ) {
      transfer( config::system_account_name, a, core_from_string( "10000.0000" ) );
      BOOST_REQUIRE_EQUAL( core_from_string( "10000.0000" ), get_balance(a) );
   }
   create_accounts_with_resources( { N(producer) } );
   BOOST_REQUIRE_EQUAL( success(), regproducer( N(producer) ) );

   produce_block();
   // stake but but not enough to go live
   stake_with_transfer( config::system_account_name, "bob",  core_from_string( "35000000.0000" ), core_from_string( "35000000.0000" ) );
   stake_with_transfer( config::system_account_name, "carl", core_from_string( "35000000.0000" ), core_from_string( "35000000.0000" ) );
   BOOST_REQUIRE_EQUAL( success(), vote( N(bob), { N(producer) } ) );
   BOOST_REQUIRE_EQUAL( success(), vote( N(carl), { N(producer) } ) );

   // start bids
   bidname( "bob",  "prefa", core_from_string("1.0003") );
   BOOST_REQUIRE_EQUAL( core_from_string( "9998.9997" ), get_balance("bob") );
   bidname( "bob",  "prefb", core_from_string("1.0000") );
   bidname( "bob",  "prefc", core_from_string("1.0000") );
   BOOST_REQUIRE_EQUAL( core_from_string( "9996.9997" ), get_balance("bob") );

   bidname( "carl", "prefd", core_from_string("1.0000") );
   bidname( "carl", "prefe", core_from_string("1.0000") );
   BOOST_REQUIRE_EQUAL( core_from_string( "9998.0000" ), get_balance("carl") );

   BOOST_REQUIRE_EQUAL( error("assertion failure with message: account is already highest bidder"),
                        bidname( "bob", "prefb", core_from_string("1.1001") ) );
   BOOST_REQUIRE_EQUAL( error("assertion failure with message: must increase bid by 10%"),
                        bidname( "alice", "prefb", core_from_string("1.0999") ) );
   BOOST_REQUIRE_EQUAL( core_from_string( "9996.9997" ), get_balance("bob") );
   BOOST_REQUIRE_EQUAL( core_from_string( "10000.0000" ), get_balance("alice") );

   // alice outbids bob on prefb
   {
      const asset initial_names_balance = get_balance(N(eosio.names));
      BOOST_REQUIRE_EQUAL( success(),
                           bidname( "alice", "prefb", core_from_string("1.1001") ) );
      BOOST_REQUIRE_EQUAL( core_from_string( "9997.9997" ), get_balance("bob") );
      BOOST_REQUIRE_EQUAL( core_from_string( "9998.8999" ), get_balance("alice") );
      BOOST_REQUIRE_EQUAL( initial_names_balance + core_from_string("0.1001"), get_balance(N(eosio.names)) );
   }

   // david outbids carl on prefd
   {
      BOOST_REQUIRE_EQUAL( core_from_string( "9998.0000" ), get_balance("carl") );
      BOOST_REQUIRE_EQUAL( core_from_string( "10000.0000" ), get_balance("david") );
      BOOST_REQUIRE_EQUAL( success(),
                           bidname( "david", "prefd", core_from_string("1.9900") ) );
      BOOST_REQUIRE_EQUAL( core_from_string( "9999.0000" ), get_balance("carl") );
      BOOST_REQUIRE_EQUAL( core_from_string( "9998.0100" ), get_balance("david") );
   }

   // eve outbids carl on prefe
   {
      BOOST_REQUIRE_EQUAL( success(),
                           bidname( "eve", "prefe", core_from_string("1.7200") ) );
   }

   produce_block( fc::days(14) );
   produce_block();

   // highest bid is from david for prefd but no bids can be closed yet
   BOOST_REQUIRE_EXCEPTION( create_account_with_resources( N(prefd), N(david) ),
                            fc::exception, fc_assert_exception_message_is( not_closed_message ) );

   // stake enough to go above the 15% threshold
   stake_with_transfer( config::system_account_name, "alice", core_from_string( "10000000.0000" ), core_from_string( "10000000.0000" ) );
   BOOST_REQUIRE_EQUAL(0, get_producer_info("producer")["unpaid_blocks"].as<uint32_t>());
   BOOST_REQUIRE_EQUAL( success(), vote( N(alice), { N(producer) } ) );

   // need to wait for 14 days after going live
   produce_blocks(10);
   produce_block( fc::days(2) );
   produce_blocks( 10 );
   BOOST_REQUIRE_EXCEPTION( create_account_with_resources( N(prefd), N(david) ),
                            fc::exception, fc_assert_exception_message_is( not_closed_message ) );
   // it's been 14 days, auction for prefd has been closed
   produce_block( fc::days(12) );
   create_account_with_resources( N(prefd), N(david) );
   produce_blocks(2);
   produce_block( fc::hours(23) );
   // auctions for prefa, prefb, prefc, prefe haven't been closed
   BOOST_REQUIRE_EXCEPTION( create_account_with_resources( N(prefa), N(bob) ),
                            fc::exception, fc_assert_exception_message_is( not_closed_message ) );
   BOOST_REQUIRE_EXCEPTION( create_account_with_resources( N(prefb), N(alice) ),
                            fc::exception, fc_assert_exception_message_is( not_closed_message ) );
   BOOST_REQUIRE_EXCEPTION( create_account_with_resources( N(prefc), N(bob) ),
                            fc::exception, fc_assert_exception_message_is( not_closed_message ) );
   BOOST_REQUIRE_EXCEPTION( create_account_with_resources( N(prefe), N(eve) ),
                            fc::exception, fc_assert_exception_message_is( not_closed_message ) );
   // attemp to create account with no bid
   BOOST_REQUIRE_EXCEPTION( create_account_with_resources( N(prefg), N(alice) ),
                            fc::exception, fc_assert_exception_message_is( "no active bid for name" ) );
   // changing highest bid pushes auction closing time by 24 hours
   BOOST_REQUIRE_EQUAL( success(),
                        bidname( "eve",  "prefb", core_from_string("2.1880") ) );

   produce_block( fc::hours(22) );
   produce_blocks(2);

   BOOST_REQUIRE_EXCEPTION( create_account_with_resources( N(prefb), N(eve) ),
                            fc::exception, fc_assert_exception_message_is( not_closed_message ) );
   // but changing a bid that is not the highest does not push closing time
   BOOST_REQUIRE_EQUAL( success(),
                        bidname( "carl", "prefe", core_from_string("2.0980") ) );
   produce_block( fc::hours(2) );
   produce_blocks(2);
   // bid for prefb has closed, only highest bidder can claim
   BOOST_REQUIRE_EXCEPTION( create_account_with_resources( N(prefb), N(alice) ),
                            eosio_assert_message_exception, eosio_assert_message_is( "only highest bidder can claim" ) );
   BOOST_REQUIRE_EXCEPTION( create_account_with_resources( N(prefb), N(carl) ),
                            eosio_assert_message_exception, eosio_assert_message_is( "only highest bidder can claim" ) );
   create_account_with_resources( N(prefb), N(eve) );

   BOOST_REQUIRE_EXCEPTION( create_account_with_resources( N(prefe), N(carl) ),
                            fc::exception, fc_assert_exception_message_is( not_closed_message ) );
   produce_block();
   produce_block( fc::hours(24) );
   // by now bid for prefe has closed
   create_account_with_resources( N(prefe), N(carl) );
   // prefe can now create *.prefe
   BOOST_REQUIRE_EXCEPTION( create_account_with_resources( N(xyz.prefe), N(carl) ),
                            fc::exception, fc_assert_exception_message_is("only suffix may create this account") );
   transfer( config::system_account_name, N(prefe), core_from_string("10000.0000") );
   create_account_with_resources( N(xyz.prefe), N(prefe) );

   // other auctions haven't closed
   BOOST_REQUIRE_EXCEPTION( ( N(prefa), N(bob) ),
                            fc::exception, fc_assert_exception_message_is( not_closed_message ) );

} FC_LOG_AND_RETHROW()
**/
BOOST_FIXTURE_TEST_CASE( move_producers_in_and_out, eosio_system_tester ) try {

   const asset net = core_from_string("80.0000");
   const asset cpu = core_from_string("80.0000");

   // create accounts {defproducera, defproducerb, ..., defproducerz} and register as producers
   std::vector<account_name> producer_names;
   {
      producer_names.reserve('z' - 'a' + 1);
      const std::string root("defproducer");
      for ( char c = 'a'; c <= 'z'; ++c ) {
         producer_names.emplace_back(root + std::string(1, c));
      }
      setup_producer_accounts(producer_names);
      for (const auto& p: producer_names) {
         BOOST_REQUIRE_EQUAL( success(), regproducer(p) );
         produce_blocks(1);
         ilog( "------ get pro----------" );
         wdump((p));
      }
   }

   // give a chance for everyone to produce blocks
   {
      produce_blocks(23 * 12 + 20);
      bool all_21_produced = true;
      for (uint32_t i = 0; i < 21; ++i) {
         if (0 == get_producer_info(producer_names[i])["produced_blocks"].as<uint32_t>()) {
            all_21_produced = false;
         }
      }
      //BOOST_REQUIRE(all_21_produced && rest_didnt_produce);
   }

   // TODO test removing producer from schedule

   {
      produce_block(fc::hours(7));
   }

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( setparams, eosio_system_tester ) try {
   //install multisig contract
   abi_serializer msig_abi_ser = initialize_multisig();
   auto producer_names = active_and_vote_producers();

   //helper function
   auto push_action_msig = [&]( const account_name& signer, const action_name &name, const variant_object &data, bool auth = true ) -> action_result {
         string action_type_name = msig_abi_ser.get_action_type(name);

         action act;
         act.account = N(eosio.msig);
         act.name = name;
         act.data = msig_abi_ser.variant_to_binary( action_type_name, data, abi_serializer_max_time );

         return base_tester::push_action( std::move(act), auth ? uint64_t(signer) : signer == N(bob111111111) ? N(alice1111111) : N(bob111111111) );
   };

   // test begins
   vector<permission_level> prod_perms;
   for ( auto& x : producer_names ) {
      prod_perms.push_back( { name(x), config::active_name } );
   }

   eosio::chain::chain_config params;
   params = control->get_global_properties().configuration;
   //change some values
   params.max_block_net_usage += 10;
   params.max_transaction_lifetime += 1;

   transaction trx;
   {
      variant pretty_trx = fc::mutable_variant_object()
         ("expiration", "2020-01-01T00:30")
         ("ref_block_num", 2)
         ("ref_block_prefix", 3)
         ("max_net_usage_words", 0)
         ("max_cpu_usage_ms", 0)
         ("delay_sec", 0)
         ("actions", fc::variants({
               fc::mutable_variant_object()
                  ("account", name(config::system_account_name))
                  ("name", "setparams")
                  ("authorization", vector<permission_level>{ { config::system_account_name, config::active_name } })
                  ("data", fc::mutable_variant_object()
                   ("params", params)
                  )
                  })
         );
      abi_serializer::from_variant(pretty_trx, trx, get_resolver(), abi_serializer_max_time);
   }

   BOOST_REQUIRE_EQUAL(success(), push_action_msig( N(alice1111111), N(propose), mvo()
                                                    ("proposer",      "alice1111111")
                                                    ("proposal_name", "setparams1")
                                                    ("trx",           trx)
                                                    ("requested", prod_perms)
                       )
   );

   // get 16 approvals
   for ( size_t i = 0; i < 15; ++i ) {
      BOOST_REQUIRE_EQUAL(success(), push_action_msig( name(producer_names[i]), N(approve), mvo()
                                                       ("proposer",      "alice1111111")
                                                       ("proposal_name", "setparams1")
                                                       ("level",         permission_level{ name(producer_names[i]), config::active_name })
                          )
      );
   }

   transaction_trace_ptr trace;
   control->applied_transaction.connect([&]( const transaction_trace_ptr& t) { if (t->scheduled) { trace = t; } } );
   BOOST_REQUIRE_EQUAL(success(), push_action_msig( N(alice1111111), N(exec), mvo()
                                                    ("proposer",      "alice1111111")
                                                    ("proposal_name", "setparams1")
                                                    ("executer",      "alice1111111")
                       )
   );

   BOOST_REQUIRE( bool(trace) );
   BOOST_REQUIRE_EQUAL( 1, trace->action_traces.size() );
   BOOST_REQUIRE_EQUAL( transaction_receipt::executed, trace->receipt->status );

   produce_blocks( 250 );

   // make sure that changed parameters were applied
   auto active_params = control->get_global_properties().configuration;
   BOOST_REQUIRE_EQUAL( params.max_block_net_usage, active_params.max_block_net_usage );
   BOOST_REQUIRE_EQUAL( params.max_transaction_lifetime, active_params.max_transaction_lifetime );

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( setram_effect, eosio_system_tester ) try {

   const asset net = core_from_string("8.0000");
   const asset cpu = core_from_string("8.0000");
   std::vector<account_name> accounts = { N(aliceaccount), N(bobbyaccount) };
   for (const auto& a: accounts) {
      create_account_with_resources(a, config::system_account_name, core_from_string("1000.0000"), false, net, cpu);
   }

   {
      const auto name_a = accounts[0];
      transfer( config::system_account_name, name_a, core_from_string("1000.0000") );
      BOOST_REQUIRE_EQUAL( core_from_string("1000.0000"), get_balance(name_a) );
      const uint64_t init_bytes_a = get_total_stake(name_a)["ram_bytes"].as_uint64();
      BOOST_REQUIRE_EQUAL( success(), buyram( name_a, name_a, core_from_string("300.0000") ) );
      BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance(name_a) );
      const uint64_t bought_bytes_a = get_total_stake(name_a)["ram_bytes"].as_uint64() - init_bytes_a;

      // after buying and selling balance should be 700 + 300 * 0.995 * 0.995 = 997.0075 (actually 997.0074 due to rounding fees up)
      BOOST_REQUIRE_EQUAL( success(), sellram(name_a, bought_bytes_a ) );
      auto refund = get_refund_request( name_a );
      BOOST_REQUIRE_EQUAL( core_from_string("1000.0000"), get_balance(name_a) + refund["ram_amount"].as<asset>());
   }

   {
      const auto name_b = accounts[1];
      transfer( config::system_account_name, name_b, core_from_string("1000.0000") );
      BOOST_REQUIRE_EQUAL( core_from_string("1000.0000"), get_balance(name_b) );
      const uint64_t init_bytes_b = get_total_stake(name_b)["ram_bytes"].as_uint64();
      // name_b buys ram at current price
      BOOST_REQUIRE_EQUAL( success(), buyram( name_b, name_b, core_from_string("300.0000") ) );
      BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance(name_b) );
      const uint64_t bought_bytes_b = get_total_stake(name_b)["ram_bytes"].as_uint64() - init_bytes_b;

      // Mainnet: increase max_ram_size, ram bought by name_b loses part of its value
      // Worbli: Increase in RAM should not affect value as sell price is calulated by resource usage and not max_ram
      BOOST_REQUIRE_EQUAL( wasm_assert_msg("ram may only be increased"),
                           push_action(config::system_account_name, N(setram), mvo()("max_ram_size", 64ll*1024 * 1024 * 1024)) );
      BOOST_REQUIRE_EQUAL( error("missing authority of eosio"),
                           push_action(name_b, N(setram), mvo()("max_ram_size", 80ll*1024 * 1024 * 1024)) );
      BOOST_REQUIRE_EQUAL( success(),
                           push_action(config::system_account_name, N(setram), mvo()("max_ram_size", 80ll*1024 * 1024 * 1024)) );

      BOOST_REQUIRE_EQUAL( success(), sellram(name_b, bought_bytes_b ) );
      
      auto refund = get_refund_request( name_b );
      BOOST_REQUIRE_EQUAL( core_from_string("1000.0000"), get_balance(name_b) + refund["ram_amount"].as<asset>());
   }

} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
