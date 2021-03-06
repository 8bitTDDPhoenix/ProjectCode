library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.all;



entity wrong_code is
       port(
		     clk : in std_logic;
			  reset : in std_logic;
			  err_event : in std_logic;
			  red : out std_logic_vector(14 downto 11);
			  failed : out std_logic
);
end wrong_code;

architecture arc of wrong_code is
   type state is (err_0, err_1, err_2, err_3);
	signal present_state, next_state : state;

begin

   state_reg : process (clk)
	 begin
	      if rising_edge(clk) then
			   if reset = '0' then
                present_state <= err_0;
            else
                present_state <= next_state;
            end if;
        end if;
    end process;
	 
	 outputs : process (present_state)
	 begin
	      case  present_state is
			
			 when err_0 =>
			 red(14 downto 11) <= "0000";
			 failed <= '0';
			 
			 when err_1 =>
			 red (14 downto 11) <= "0001";
			 failed <= '0';
			 
			 when err_2 =>
				RED(14 downto 11) <= "0011";
				failed <= '0';
				
			when err_3 =>
			RED(14 downto 11) <= "1111";
			failed <= '1';
		end case;
	end process;

 nxt_state : process (present_state, err_event)
    begin
        next_state <= present_state;
        case present_state is
            when err_0 =>
                if err_event = '1' then
                    next_state <= err_1;
                end if;
            when err_1 =>
                if err_event = '1' then
                    next_state <= err_2;
                end if;
            when err_2 =>
                if err_event = '1' then
                    next_state <= err_3;
                end if;
            when err_3 =>
                next_state <= err_3; 
            when others => 
                next_state <= err_0;
        end case;
    end process;
end arc;