/*module address(CLK,add);
input CLK;
output add;
reg [7:0] add=8'b0;
always @ (posedge CLK)
begin
  add[7:0]<=add[7:0]+1'b1;
  if(add==8'd255) add<=8'b0;
end
endmodule 
*/
module address(CLK,add,rst);
input rst;
input CLK;
output add;
reg [7:0] add=8'd255;
always @ (posedge CLK)
begin
	if(add==8'd255) add<=8'd255;
	else 	add[7:0]<=add[7:0]+1'b1;
	if(rst==1&&add==8'd255) add<=8'b0;
end
endmodule 