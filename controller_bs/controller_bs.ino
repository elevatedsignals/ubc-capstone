#include "constants_bs.h"
#include "MiniW5100.h"

EthernetClient client; // struct for functions that manipulate communication with server

void setup() {
  Serial.begin(9600);
  int error = FALSE;

  client = init_ethernet(client);
  print_ip();

  client = check_recv_buffer(client, &error);

  struct TH t_h_data = {1, 2}; // REPLACE TEST VALUE
  float co2_conc_data = 3; // REPLACE TEST VALUE
  bool airflow_data = TRUE; // REPLACE TEST VALUE

  String data = prepare_payload(t_h_data, co2_conc_data, airflow_data, &error);

  if(error) {
    Serial.println(ERROR_PREP_PAYLOAD);
    error = FALSE;
  } else {
    client = make_http_request(client, data, &error);

    if(error) {
      Serial.println(ERROR_HTTP_REQUEST);
      error = FALSE;
    }
  }

  client = check_connection(client, &error);

  if(error) {
    Serial.println(ERROR_CONNECTION);
    error = FALSE;
  }

  validate_ip();
  print_ip();
}

void loop() {

}
