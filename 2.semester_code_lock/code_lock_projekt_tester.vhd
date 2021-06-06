library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.all;

entity code_lock_projekt_tester is
      port(
		  CLOCK_50 : in std_logic;
		  KEY : in std_logic_vector(1 downto 0);
		  SW : in std_logic_vector(8 downto 5);
		  LEDG : out std_logic_vector(8 downto 0);
		  LEDR : out std_logic_vector (17 downto 0)
		);
end code_lock_projekt_tester;

architecture arc of code_lock_projekt_tester is
signal err_event_sig, failed_sig : std_logic;

begin

U1: entity work.code_lock_projekt port map
    (
	  clk => CLOCK_50,
	  reset => KEY(1),
	  enter => KEY(0),
	  failed => failed_sig,
	  code => SW(8 downto 5),
	  
	  deactivated => LEDR(5 downto 1),
	  activated => LEDG(5 downto 1),
	  lock => LEDG(0),
	  permanently_lock_out => LEDR(0),
	  GREEN => LEDG(8 downto 8),
	  err_event => err_event_sig
	 );
U2: entity work.wrong_code port map
   (
	clk => CLOCK_50,
	reset => KEY(1),
	err_event => err_event_sig,
	red => LEDR(14 downto 11),
	failed => failed_sig
	);
end arc;	