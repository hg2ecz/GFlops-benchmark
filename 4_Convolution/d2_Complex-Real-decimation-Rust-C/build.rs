extern crate cc;

fn main() {
    cc::Build::new()
	.file("c_inc/testfunc.c")
	.file("c_inc/test.c")
	.flag("-Ofast")
	.flag("-march=native")
	.flag("-funroll-all-loops")
	.include("c_inc")
	.compile("libtestc.a");
}
