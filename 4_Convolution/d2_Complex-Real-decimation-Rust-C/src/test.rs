const DATALEN: usize = 20 * 1000 * 1024;
const COEFLEN: usize = 512;
//const DECIM: usize = 20;

#[repr(C)]
pub struct Input {
    data_real: Vec<f32>,
    data_imag: Vec<f32>,
    coeff: Vec<f32>,
}

#[repr(C)]
pub struct Output {
    data_real: Vec<f32>,
    data_imag: Vec<f32>,
}

pub fn test_init() -> Input {
    let mut data_real: Vec<f32> = Vec::with_capacity(DATALEN);
    let mut data_imag: Vec<f32> = Vec::with_capacity(DATALEN);
    let mut coeff: Vec<f32> = Vec::with_capacity(COEFLEN);
    for i in 0..DATALEN {
        data_real.push(1.0 * (2. * std::f32::consts::PI / 1001. * i as f32).sin());
        data_imag.push(1.1 * (2.131 * std::f32::consts::PI / 1001. * i as f32).sin());
    }
    for i in 0..COEFLEN {
        coeff.push(0.9 + 0.1 / (i as f32 + 1.));
    }
    Input {
        data_real,
        data_imag,
        coeff,
    }
}

pub fn test_check(output: &Output) -> f32 {
    let mut res: f32 = 0.;
    for i in 0..output.data_real.len() {
        res += output.data_real[i] + output.data_imag[i];
    }
    res
}

#[allow(improper_ctypes)]
#[link(name = "testc", kind = "static")]
extern "C" {
    pub fn testrust1(output: *mut Output, input: *const Input) -> i32;
    pub fn testrust2(output: *mut Output, input: *const Input) -> i32;
    pub fn testrust3(output: *mut Output, input: *const Input) -> i32;
    pub fn testrust4(output: *mut Output, input: *const Input) -> i32;
    pub fn testrust5(output: *mut Output, input: *const Input) -> i32;
    #[cfg(any(target_arch = "x86", target_arch = "x86_64"))]
    pub fn testrust6(output: *mut Output, input: *const Input) -> i32;
    #[cfg(any(target_arch = "x86", target_arch = "x86_64"))]
    pub fn testrust7(output: *mut Output, input: *const Input) -> i32;
}

pub fn test(func: u32, input: &Input) -> Output {
    let mut output = Output {
        data_real: Vec::with_capacity(input.data_real.len() * 2),
        data_imag: Vec::with_capacity(input.data_imag.len() * 4),
    };
    #[cfg(any(target_arch = "arm", target_arch = "aarch64"))]
    match func {
        1 => unsafe {
            testrust1(&mut output, input);
        },
        2 => unsafe {
            testrust2(&mut output, input);
        },
        3 => unsafe {
            testrust3(&mut output, input);
        },
        4 => unsafe {
            testrust4(&mut output, input);
        },
        5 => unsafe {
            testrust5(&mut output, input);
        },
        _ => (),
    }
    #[cfg(any(target_arch = "x86", target_arch = "x86_64"))]
    match func {
        1 => unsafe {
            testrust1(&mut output, input);
        },
        2 => unsafe {
            testrust2(&mut output, input);
        },
        3 => unsafe {
            testrust3(&mut output, input);
        },
        4 => unsafe {
            testrust4(&mut output, input);
        },
        5 => unsafe {
            testrust5(&mut output, input);
        },
        6 => {
            if is_x86_feature_detected!("avx") {
                unsafe {
                    testrust6(&mut output, input);
                }
            }
        }
        7 => {
            if is_x86_feature_detected!("avx") {
                unsafe {
                    testrust7(&mut output, input);
                }
            }
        }
        _ => (),
    }
    output
}
