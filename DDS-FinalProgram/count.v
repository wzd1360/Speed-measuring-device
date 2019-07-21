module count(clk_in,clk_out);
input clk_in;
output reg clk_out=1'b0;
reg [31:0] cnt=32'b0;
always@(posedge clk_in)
begin 
if(cnt==100000/2)
begin 
clk_out<=!clk_out;
cnt<=32'b0;
end 
else cnt<=cnt+1'b1;
end 

endmodule  