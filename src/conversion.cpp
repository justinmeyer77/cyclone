namespace conversion{

  constexpr int from_herz_to_microseconds (int herz) {
    return 1/herz*1000*1000;
  }

}
