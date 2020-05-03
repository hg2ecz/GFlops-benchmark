mod test;
use std::time::Instant;
use test::{Input, Output};

fn testfn(tf: fn(&Input) -> Output, input: &Input, tname: &str) {
    let start_time = Instant::now();
    let output = tf(&input);
    let elapsed_time = start_time.elapsed();

    let chk = test::test_check(&output);
    let msec = elapsed_time.as_secs() * 1000 + elapsed_time.subsec_millis() as u64;
    println!("{} --> eltime: {} ms, result: {}", tname, msec, chk);
}

fn main() {
    let input = test::test_init();
    testfn(test::test1, &input, "testR1");
    testfn(test::test2, &input, "testR2");
    testfn(test::test3, &input, "testR3");
}
