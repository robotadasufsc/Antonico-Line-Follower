#define kc_dir 15.0
#define ti_dir 0.2
#define td_dir 0  

#define kc_esq 15.0
#define ti_esq 0.2
#define td_esq 0

#define Ts 0.02
#define umin 0.0
#define umax 5.0

//Modelo Esquerdo
//float kpesq = (4.86*(500/255)), t1esq = (1.06/3), tetaesq = .02;

//Modelo Direito
//float kpdir = (5.15*(500/255)), t1dir = (0.8/3), tetadir = .02;

class Controller {
    float erro_k1 = 0, u_k1 = 0, ui_k1 = 0;
    float kc, ti, td, ts;
  public:
    
    Controller(float kc, float ti, float td, float ts)
    {
      this->kc = kc;
      this->ti = ti;
      this->td = td;
      this->ts = ts;
    }

    float update (float setpoint, float current_value)
    /* Returns control output */
    {
      float erro,up,ui,ud;
      erro = setpoint - current_value;
      up = kc*erro;                           //termo proporcional
      ui = ui_k1 + ((kc*ts)/ti)*erro_k1;      //termo integral
      ud = ((kc*td)/ts)*(erro - erro_k1);     //termo derivativo
      
      float u_total = up + ui + ud;
      
      u_total = min(umax,max(u_total,umin)); //saturação

      erro_k1 = erro;
      ui_k1 = ui;

      return u_total;
    }
};

Controller controle_esq = Controller(kc_esq, ti_esq, td_esq, Ts);
Controller controle_dir = Controller(kc_dir, ti_dir, td_dir, Ts);
