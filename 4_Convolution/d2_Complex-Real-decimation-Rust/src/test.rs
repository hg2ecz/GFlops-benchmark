const DATALEN: usize = 20 * 1000 * 1024;
const COEFLEN: usize = 512;
const DECIM: usize = 20;

pub struct Input {
    data_real: Vec<f32>,
    data_imag: Vec<f32>,
    coeff: Vec<f32>,
}

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

pub fn test1(input: &Input) -> Output {
    let mut data_real: Vec<f32> = Vec::with_capacity(input.data_real.len());
    let mut data_imag: Vec<f32> = Vec::with_capacity(input.data_imag.len());

    for i in (0..input.data_real.len() - input.coeff.len() - DECIM + 2).step_by(DECIM) {
        let mut res_re: f32 = 0.;
        let mut res_im: f32 = 0.;
        for j in 0..input.coeff.len() {
            res_re += input.data_real[i + j] * input.coeff[j];
            res_im += input.data_imag[i + j] * input.coeff[j];
        }
        data_real.push(res_re);
        data_imag.push(res_im);
    }
    Output {
        data_real,
        data_imag,
    }
}

pub fn test2(input: &Input) -> Output {
    let mut data_real: Vec<f32> = Vec::with_capacity(input.data_real.len());
    let mut data_imag: Vec<f32> = Vec::with_capacity(input.data_imag.len());

    for i in (0..=input.data_real.len() - input.coeff.len() - DECIM + 2).step_by(DECIM) {
        let mut res_re: f32 = 0.;
        let mut res_im: f32 = 0.;
        for j in 0..input.coeff.len() {
            let coeff = unsafe { input.coeff.get_unchecked(j) };
            res_re += unsafe { input.data_real.get_unchecked(i + j) } * coeff;
            res_im += unsafe { input.data_imag.get_unchecked(i + j) } * coeff;
        }
        data_real.push(res_re);
        data_imag.push(res_im);
    }
    Output {
        data_real,
        data_imag,
    }
}

pub fn test3(input: &Input) -> Output {
    let mut data_real: Vec<f32> = Vec::with_capacity(input.data_real.len());
    let mut data_imag: Vec<f32> = Vec::with_capacity(input.data_imag.len());

    for i in (0..input.data_real.len() - input.coeff.len() - DECIM + 2).step_by(DECIM) {
        data_real.push(
            input
                .coeff
                .iter()
                .zip(&input.data_real[i..])
                .map(|(co, sa)| co * sa)
                .sum(),
        );
        data_imag.push(
            input
                .coeff
                .iter()
                .zip(&input.data_imag[i..])
                .map(|(co, sa)| co * sa)
                .sum(),
        );
    }
    Output {
        data_real,
        data_imag,
    }
}
