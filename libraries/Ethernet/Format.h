#ifndef FORMAT_H
#define FORMAT_H

/*
*  Purpose: Formats sensor data for upload
*/
char* prepare_payload(int cap_id, short val, char *time_string) {

  char payload[100] = "";

  switch(cap_id) {
    case 58: { // ID_TEMP
      char array[6];
      dtostrf(val, 1, 1, array);
      strcpy(payload, "{\"capability_id\": 58,\"json_value\":{\"temperature\":");
      strcat(payload, array);
      strcat(payload, "}");
      break;
    }

    case 43: { // ID_HUM
      char array[6];
      dtostrf(val, 1, 1, array);
      strcpy(payload, "{\"capability_id\": 43,\"json_value\":{\"humidity\":");
      strcat(payload, array);
      strcat(payload, "}");
      break;
    }

    case 13: { // ID_CO2
      char array[6];
      dtostrf(val, 1, 1, array);
      strcpy(payload, "{\"capability_id\": 13,\"json_value\":{\"carbonDioxide\":");
      strcat(payload, array);
      strcat(payload, "}");
      break;
    }

    case 29: { // ID_PAR
      char array[6];
      dtostrf(val, 1, 1, array);
      strcpy(payload, "{\"capability_id\": 29,\"json_value\":{\"value\":");
      strcat(payload, array);
      strcat(payload, "}");
      break;
    }
    case 1: { // ID_AF
      strcpy(payload, "{\"capability_id\": 1,\"json_value\":{\"state\":");

      if(val == 0.0) {
        char *array = "\"still\"";
        strcat(payload, array);
      } else {
        char *array = "\"moving\"";
        strcat(payload, array);
      }

      strcat(payload, "}");
      break;
    }
  }

  if(time_string != NULL) {
    strcat(payload, "\"timestamp\": \"");
    strcat(payload, time_string);
    strcat(payload, "\"");
  }

  strcat(payload, "}");

  return payload;
}

#endif
