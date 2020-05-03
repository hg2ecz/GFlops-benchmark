mod test;
use std::time::Instant;
use test::Input;

fn testfn(tf: u32, input: &Input, tname: &str) {
    let start_time = Instant::now();
    let output = test::test(tf, input);
    let elapsed_time = start_time.elapsed();

    let chk = test::test_check(&output);
    let msec = elapsed_time.as_secs() * 1000 + elapsed_time.subsec_millis() as u64;
    println!("{} --> eltime: {} ms, result: {}", tname, msec, chk);
}

fn main() {
    let input = test::test_init();
    testfn(1, &input, "testRC1");
    testfn(2, &input, "testRC2");
    testfn(3, &input, "testRC3");
    testfn(4, &input, "testRC4");
    testfn(5, &input, "testRC5");
    #[cfg(any(target_arch = "x86", target_arch = "x86_64"))]
    {
        testfn(6, &input, "tAVCRC6");
        testfn(7, &input, "tAVXRC7");
    }
}
