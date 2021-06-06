library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.all;



entity code_lock_projekt is
    port (
        clk 			: in std_logic;
		  GREEN     	: out std_logic_vector(9 downto 9);
        reset 			: in std_logic;
        code 			: in std_logic_vector(3 downto 0);
        enter 			: in std_logic;
		  deactivated	: out std_logic_vector(5 downto 1);
		  activated		: out std_logic_vector(5 downto 1);
        failed 		: in std_logic;
		  access_int   : out std_logic;
        lock 			: out std_logic;
        permanently_lock_out : out std_logic;
        err_event 	: out std_logic
    );
end code_lock_projekt;

architecture arc of code_lock_projekt is
    -- vi ændre det således vi kan se hvornår systemet er aktivt eller ik
    type state is (idle_state, evaluating1_state, unlocked_state, going_idle_state, wrong_code_state, permanently_locked_state);
    signal present_state, next_state : state;
	 signal lock_sig: std_logic;
    constant code1 : std_logic_vector(3 downto 0) := "1000"; -- korrekte kode for systemet.
	 constant system_deactivated_led_off: std_logic_vector(5 downto 1) := "00000";
	 constant system_deactivated_led_on: std_logic_vector(5 downto 1)  := "11111";
	 constant system_activated_led_off: std_logic_vector(5 downto 1)    := "00000";
	 constant system_activated_led_on: std_logic_vector(5 downto 1)     := "11111";
begin
    state_reg : process (clk)
    begin
        if rising_edge(clk) then
            if reset = '0' then
                present_state <= idle_state;
            else
                present_state <= next_state;
            end if;
        end if;
    end process;

    outputs : process (present_state, enter, code)
    begin
        
		  
        err_event <= '0';
        permanently_lock_out <= '0';
		  GREEN <= "0"; --står som standart til ikke at lyse

        case present_state is

            when unlocked_state => 
                lock <= '0';
					 lock_sig <= '0';
					 GREEN <= "1"; --Lys grøn hvis koden er rigtig
					  

            when evaluating1_state => 
                if enter = '1' and code /= code1 then
                    err_event <= '1';
						  lock <= '1';
					     lock_sig <= '1';
                end if;
 
            when permanently_locked_state => 
                permanently_lock_out <= '1';
					 lock_sig <='1';
					 lock <= '1';
					 
				when wrong_code_state =>
					lock <= '1';
					lock_sig <= '1';
					GREEN <= "0"; ----Lys ikke grøn hvis en kode er forkert

            when others => 
                lock <= '1';
					 lock_sig <='1';
                permanently_lock_out <= '0';

        end case;
    end process;

    nxt_state : process (present_state, enter, code, failed)
    begin
        next_state <= present_state;
        case present_state is

            when idle_state => 
                if enter = '0' then
                    next_state <= evaluating1_state;
                end if;

            when evaluating1_state => 
                if code = code1 and enter = '1' then
                    next_state <= unlocked_state; -- rigtig kode indtastet
                elsif enter = '1' then
                    next_state <= wrong_code_state; -- forkert kode indtastet, hop ned i Wrong code state
                end if;

            when unlocked_state => 
                if enter = '0' then
                    next_state <= going_idle_state;
                end if;

            when going_idle_state => 
                if enter = '1' then
                    next_state <= idle_state;
                end if;

            when wrong_code_state => 
                if enter = '0' then
                    if failed = '1' then
                        next_state <= permanently_locked_state;
                    else
                        next_state <= going_idle_state;
                    end if;
                end if;

            when permanently_locked_state => 
                next_state <= permanently_locked_state; -- bliv i denne tilstand

            when others => 
                next_state <= idle_state;

        end case;
    end process;

	 activated_deactivated: process(lock_sig)
	 begin 
	 case lock_sig is
	 when '0' => deactivated <= system_deactivated_led_on;
	 activated <= system_activated_led_off;
	 access_int <= '0';
	 
	 when '1' => activated <= system_activated_led_on;
	 deactivated <= system_deactivated_led_off;
	 access_int <= '1';
	 end case;
	 end process;
	 

	 
end arc;
			 