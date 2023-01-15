
#ifndef BCDFLOAT_LEN
    #define BCDFLOAT_LEN 16
#endif

class bcdFloat {

  public:
    bcdFloat(long num);
    void set(long num);
    void setFromString(String num, uint8_t point);
    uint8_t length();
    void approx(uint8_t point);
    uint8_t getFreeZerosNum();
    long int toInt();
    String toString(uint8_t point, uint8_t acc, uint8_t ext);
    void add(uint8_t num, uint8_t point);
    void sub(uint8_t num, uint8_t point);
    void clearZeros();
    uint8_t digitAt(uint8_t index);
  private:
    int8_t _dig[BCDFLOAT_LEN];
    uint8_t _len;
};



bcdFloat::bcdFloat(long num) {
    this->set(num);
}

void bcdFloat::set(long num) {
  char c;

  _len = 0;
  String s = String(num);
  for (int i = s.length()-1; i>=0; i--) {
    c = s.charAt(i);
    if (c >= '0' && c <= '9') {
      _dig[_len] = c-'0';
      _len++;        
    }
  }

  //dopełnienie zerami
  for (int i = _len; i < BCDFLOAT_LEN; i++)
    _dig[i] = 0;
}

void bcdFloat::setFromString(String num, uint8_t point) {
  
  boolean isDot = false;
  int dPos = 0;

  for (int i = 0; i < num.length(); i++) {
    if (num.charAt(i) == '.') {
      isDot = true;
      dPos = i;
      break;
    }
  }

  if (isDot) {
    //wyrzuć przecinek ze stringa
    num.remove(dPos,1);
  } else {
    dPos = num.length();
  }
  
  while (num.length() != (point+dPos)) {
    //dopełnanie zerami
    if (num.length() < dPos+point) num.concat("0"); 
    //usuwanie zer
    if (num.length() > dPos+point) num.remove(num.length()-1);
  }

  //konwersja do bcdFloat;
  char c;
  _len = 0;

  for (int i = num.length()-1; i>=0; i--) {
    c = num.charAt(i);
    if (c >= '0' && c <= '9') {
      _dig[_len] = c-'0';
      _len++;        
    }
  }

  //dopełnienie zerami
  for (int i = _len; i < BCDFLOAT_LEN; i++) {
    _dig[i] = 0;
  }
}

uint8_t bcdFloat::length() {
    return _len;
}

void bcdFloat::approx(uint8_t point) {
    for (int i = point-1; i >= 0; i--) {
        _dig[i] = 0;
    }
}

uint8_t bcdFloat::getFreeZerosNum() {
    uint8_t r = 0;

    while(r < BCDFLOAT_LEN-1) {
        if(_dig[r] != 0) break;
        r++;
    }
    return r;
}


long int bcdFloat::toInt() {
    //konwersja na liczbę całkowitą
    long int r = 0;

    for (int i = _len-1; i >= 0; i--) {
        r *= 10;
        r+=_dig[i];
    }

    return r;
}

String bcdFloat::toString(uint8_t point, uint8_t acc, uint8_t ext) {
  String r = "";

  char c;
  uint8_t acnt = 0;

  //length extention

  
  uint8_t elen = _len + ext;

  //rozszerzanie o zera, jeśli długość jest mniejsza od liczby miejsc po przecinku
  if (elen <= point) elen = point + 1;

  //gdy przesadzimy
  if (elen > BCDFLOAT_LEN) elen = BCDFLOAT_LEN;

  for (int i = elen-1; i >=0; i--) {

    if(acnt > 0) {
      if (acnt > acc) break;
      acnt++;
    }

    c = _dig[i] + '0';
    r.concat(c);

    if (i == point && point != 0) {
      if (acc > 0) r.concat('.');
      acnt = 1;
    }
  }
  return r;
}

void bcdFloat::add(uint8_t num, uint8_t point) {
  if (point >= 0 && point < BCDFLOAT_LEN) {
    if(point >= _len) _len = point+1;

    _dig[point] += num;
    if (_dig[point] >= 10) {
      //carry
      _dig[point]-=10;
      this->add(1,point+1);
    }
  }
}

void bcdFloat::sub(uint8_t num, uint8_t point) {
  if (point >= 0 && point < BCDFLOAT_LEN) {

    if (_dig[point] < num) {
      //borrow    
      _dig[point]+=10;
      this->sub(1,point+1);
    }
    
    _dig[point] -= num;
    this->clearZeros();
  } else {
      this->set(0);
  }
}

void bcdFloat::clearZeros() {
  //usuwanie zer na początku
  if(_dig[_len-1] == 0 && _len > 1) {
    _len--;
    this->clearZeros();
  }
}

uint8_t bcdFloat::digitAt(uint8_t index) {
  return _dig[index];
}