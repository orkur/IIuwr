// D latch (gate model)
module d_latch(
  input d, e,
  output q, nq
);

  logic s, r, nd;

  nor g1(q, r, nq);
  nor g2(nq, s, q);
  and g3(r, e, nd);
  and g4(s, e, d);
  not g5(nd, d);

endmodule

// master-slave D flip-flop
module dff_master_slave(
  input clk, d,
  output o, o2
);

  logic q, nq1, nq2, nclk;

  d_latch dl1(d, clk, q, nq1);
  d_latch dl2(q, nclk, o, o2);
  not g(nclk, clk);

endmodule

module T(
  input clk, refresh,refresh2, 
  output w
);
	logic kabelek_or, kabelek, wch, kabelek2_or, kabelek2;
  dff_master_slave first(clk,kabelek, wch, kabelek_or);
  or o(kabelek, refresh, kabelek_or);
  dff_master_slave second(wch,kabelek2, w, kabelek2_or);
  or o2(kabelek2, refresh2, kabelek2_or);
endmodule
