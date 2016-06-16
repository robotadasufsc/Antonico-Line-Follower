#define kcdir 15.0
#define tidir 0.2
#define tddir 0  

#define kcesq 15.0
#define tiesq 0.2
#define tdesq 0

#define Ts 0.02
#define umin 0.0
#define umax 5.0

float ekesq, ek1esq, ubkesq, ufkesq, uk1esq, ykesq, upkesq, uikesq, uik1esq, udkesq;
float ekdir, ek1dir, ubkdir, ufkdir, uk1dir, ykdir, upkdir, uikdir, uik1dir, udkdir;

//Modelo Esquerdo
float kpesq = (4.86*(500/255)), t1esq = (1.06/3), tetaesq = .02;

//Modelo Direito
float kpdir = (5.15*(500/255)), t1dir = (0.8/3), tetadir = .02;

typedef struct
{
  float uk1;
  float ek1;
  float uik1;
}controlP;

controlP controlaDir(float Refdir, float wdir, controlP control_DIR)
{
    uk1dir = control_DIR.uk1;
    ek1dir = control_DIR.ek1;
    uik1dir = control_DIR.uik1;
  // Medição Dir
    ykdir = wdir;
  // Erro Dir 
    ekdir = Refdir - ykdir;
  // Controle
    upkdir = kcdir*ekdir;
    uikdir = uik1dir + ((kcdir*Ts)/tidir)*ek1dir;
    udkdir = ((kcdir*tddir)/Ts)*(ekdir - ek1dir);
    ubkdir = upkdir + uikdir + udkdir;
  // ----- Saturacao na malha
     if (ubkdir <= umin) 
    {
      ufkdir = umin;
    }
       else if (ubkdir >= umax)
    {
      ufkdir = umax;
    }
       else
    {
      ufkdir = ubkdir;
    }
  control_DIR.uk1=uk1dir;
  control_DIR.ek1=ek1dir;
  control_DIR.uik1=uik1dir;
  
  dForward((int)floor(ufkdir*51));
  
  return control_DIR;
}

controlP controlaEsq(float Refesq, float wesq, controlP control_ESQ)
{
    uk1esq = control_ESQ.uk1;
    ek1esq = control_ESQ.ek1;
    uik1esq = control_ESQ.uik1;
  // Medição Esq
    ykesq = wesq;
  // Erro Esq
    ekesq = Refesq - ykesq;
  // Controle Esq
    upkesq = kcesq*ekesq;
    uikesq = uik1esq + ((kcesq*Ts)/tiesq)*ek1esq;
    udkesq = ((kcesq*tdesq)/Ts)*(ekesq - ek1esq);
    ubkesq = upkesq + uikesq + udkesq;
  // ----- Saturacao na malha Esq
     if (ubkesq <= umin) 
    {    
      ufkesq = umin;
    }
         else if (ubkesq >= umax)
    {    
      ufkesq = umax;
    }
         else
    {    
      ufkesq = ubkesq;
    }
  control_ESQ.uk1=uk1esq;
  control_ESQ.ek1=ek1esq;
  control_ESQ.uik1=uik1esq;
  
  eForward((int)floor(ufkesq*51));
  
  return control_ESQ;
}
