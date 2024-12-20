# Veitur-Meter-Reader fyrir Home Assistant / ESPHOME
Þessi kóði er ætlaður fyrir ESPHOME og les snjallmæla frá Veitum, bæði rafmagn og hita með því að sækja gögn frá P1 tenginu.
Home Assistant er með ESPHOME intergration og því auðvelt að bæta lesaranum þar inn eftir að hann hefur verið flassaður.

Kóðinn er að mestu byggður á esphome-p1reader en það var ekki stuðningur við hitaveitumælinn og tékksummu tékkið (crc) var ekki að virka rétt. Ég tók því út crc og breytti því einnig hvernig OBIS kóðinn er lesinn svo einfaldara væri að bæta inn nýjum OBIS. Búnaðurinn, rafrásin, sem les frá P1 tenginu er svipaður því sem er hjá esphome-p1reader og einnig er hægt að nota SlimmeLezer+ dongle.

![2024-05-06 19_00_32-Stillingar – Home Assistant — Mozilla Firefox](https://github.com/gardart/veitur-meter-reader/assets/3597603/e614d056-9495-4b05-a78d-5e2ff0081a92)
![2024-05-06 18_56_26-Stillingar – Home Assistant — Mozilla Firefox](https://github.com/gardart/veitur-meter-reader/assets/3597603/703e81e8-101b-4de5-b64b-cf1c2da8bab4)
![2024-05-03 20_05_25-Window](https://github.com/gardart/veitur-meter-reader/assets/3597603/11e9a3a8-2fb3-491b-9b01-b9d41aa8573b)

# Uppsetning
```shell
git clone https://github.com/gardart/veitur-meter-reader.git
esphome run veitur-meter-reader.yaml
```
Þegar búið er að tengja lesarann við p1 tengið á ISKRA mælinum þá ætti loggurinn að sýna þegar hann sendir boð um að sækja gögn og sömuleiðis gögnin (OBIS kóðana) sem mælirinn skilar. Hitaveitumælirinn (Sharky) er tengdur þráðlaust við ISKRA mælinn og sýnir hitaveitugögnin með OBIS forskeytinu 1-0. Hann birtir því sínar mælingar sem undirmælir við ISKRA. Ef engin hitaveitugögn birtast þarf mögulega að biðja Veitur um að virkja það.
```ruby
[21:13:00][D][switch:016]: 'P1 Request Trigger' Turning OFF.
[21:13:00][D][switch:055]: 'P1 Request Trigger': Sending state OFF
[21:13:09][D][switch:012]: 'P1 Request Trigger' Turning ON.
[21:13:10][D][switch:055]: 'P1 Request Trigger': Sending state ON
[21:13:10][D][MREADER:177]: Received line: /ISk5\2MIE5E-200
[21:13:10][D][MREADER:177]: Received line:
[21:13:10][D][MREADER:177]: Received line: 0-0:96.1.0(86818632)
[21:13:10][D][MREADER:063]: fullcode: 0-0:96.1.0
[21:13:10][D][MREADER:064]: value: 86818632
[21:13:10][D][MREADER:177]: Received line: 1-0:0.9.1(211309)
[21:13:10][D][MREADER:063]: fullcode: 1-0:0.9.1
[21:13:10][D][MREADER:064]: value: 211309
[21:13:10][D][MREADER:177]: Received line: 1-0:0.9.2(240501)
[21:13:10][D][MREADER:063]: fullcode: 1-0:0.9.2
[21:13:10][D][MREADER:064]: value: 240501
[21:13:10][D][MREADER:177]: Received line: 0-0:96.1.1(36303836383138363631)
[21:13:10][D][MREADER:063]: fullcode: 0-0:96.1.1
[21:13:10][D][MREADER:064]: value: 36303836383138363631
[21:13:10][D][MREADER:177]: Received line: 1-0:1.7.0(00.098*kW)
[21:13:10][D][MREADER:063]: fullcode: 1-0:1.7.0
[21:13:10][D][MREADER:064]: value: 00.098
[21:13:10][D][MREADER:177]: Received line: 1-0:2.7.0(00.000*kW)
[21:13:10][D][MREADER:063]: fullcode: 1-0:2.7.0
[21:13:10][D][MREADER:064]: value: 00.000
[21:13:10][D][MREADER:177]: Received line: 1-0:31.7.0(000*A)
[21:13:10][D][MREADER:063]: fullcode: 1-0:31.7.0
[21:13:10][D][MREADER:064]: value: 000
[21:13:10][D][MREADER:177]: Received line: 1-0:32.7.0(231.3*V)
[21:13:10][D][MREADER:063]: fullcode: 1-0:32.7.0
[21:13:10][D][MREADER:064]: value: 231.3
[21:13:10][D][MREADER:177]: Received line: 1-0:1.8.0(000821.034*kWh)
[21:13:10][D][MREADER:063]: fullcode: 1-0:1.8.0
[21:13:10][D][MREADER:064]: value: 000821.034
[21:13:10][D][MREADER:177]: Received line: 1-0:2.8.0(000000.000*kWh)
[21:13:10][D][MREADER:063]: fullcode: 1-0:2.8.0
[21:13:10][D][MREADER:064]: value: 000000.000
[21:13:10][D][MREADER:177]: Received line: 1-0:21.7.0(00.098*kW)
[21:13:10][D][MREADER:063]: fullcode: 1-0:21.7.0
[21:13:10][D][MREADER:064]: value: 00.098
[21:13:10][D][MREADER:177]: Received line: 1-0:22.7.0(00.000*kW)
[21:13:10][D][MREADER:063]: fullcode: 1-0:22.7.0
[21:13:10][D][MREADER:064]: value: 00.000
[21:13:10][D][MREADER:177]: Received line: !
[21:13:10][D][sensor:093]: 'Cumulative Active Import': Sending state 821.03400 kWh with 3 decimals of accuracy
[21:13:10][D][sensor:093]: 'Momentary Active Import': Sending state 0.09800 kW with 3 decimals of accuracy
[21:13:10][D][sensor:093]: 'Momentary Active Import Phase 1': Sending state 0.09800 kW with 3 decimals of accuracy
[21:13:10][D][sensor:093]: 'Momentary Active Import Phase 2': Sending state 0.00000 kW with 3 decimals of accuracy
[21:13:10][D][sensor:093]: 'Momentary Active Import Phase 3': Sending state 0.00000 kW with 3 decimals of accuracy
[21:13:10][D][sensor:093]: 'Voltage Phase 1': Sending state 231.30000 V with 3 decimals of accuracy
[21:13:10][D][sensor:093]: 'Voltage Phase 2': Sending state 0.00000 V with 3 decimals of accuracy
[21:13:10][D][sensor:093]: 'Voltage Phase 3': Sending state 0.00000 V with 3 decimals of accuracy
[21:13:10][D][sensor:093]: 'Current Phase 1': Sending state 0.00000 A with 3 decimals of accuracy
[21:13:10][D][sensor:093]: 'Current Phase 2': Sending state 0.00000 A with 3 decimals of accuracy
[21:13:10][D][sensor:093]: 'Current Phase 3': Sending state 0.00000 A with 3 decimals of accuracy
[21:13:10][D][sensor:093]: 'Hot Water Volume': Sending state 0.00000 m³ with 3 decimals of accuracy
[21:13:10][D][sensor:093]: 'Hot Water Flow Rate': Sending state 0.00000 m³/h with 3 decimals of accuracy
[21:13:10][D][sensor:093]: 'Inlet Water Temperature': Sending state 0.00000 °C with 1 decimals of accuracy
[21:13:10][D][sensor:093]: 'Outlet Water Temperature': Sending state 0.00000 °C with 1 decimals of accuracy
[21:13:10][D][sensor:093]: 'Energy Content Volume': Sending state 0.00000 kWh with 3 decimals of accuracy
[21:13:10][D][sensor:093]: 'Energy Flow Rate': Sending state 0.00000 kW with 3 decimals of accuracy
```

## Dæmi um hrá gögn frá mælinum með hitaveitumælinn tengdan sem undirmælir
```
/ISk5\2MIE5E-200
0-0:96.1.0(86818123)
0-0:96.1.1(36303836383138373123)
1-0:0.9.1(222114)
1-0:0.9.2(240212)
1-0:1.7.0(00.588*kW)* ()
1-0:2.7.0(00.000*kW)
1-0:31.7.0(002*A)
1-0:32.7.0(232.5*V)
1-0:1.8.0(001988.284*kWh)
1-0:2.8.0(000000.000*kWh)
1-0:21.7.0(00.583*kW)
1-0:22.7.0(00.000*kW)
0-1:24.2.1(0366.475*m3)
0-1:24.2.10(0000000)
0-1:24.2.2(0000.090*m3/h)
0-1:24.2.3(000074.8*degC)
0-1:24.2.4(000031.3*degC)
0-1:24.2.5(0019.078*Wh)
0-1:24.2.6(0019.078*Wh)
```

# Fleiri myndir úr Home Assistant

![2024-05-05 20_39_02-Stillingar – Home Assistant — Mozilla Firefox](https://github.com/gardart/veitur-meter-reader/assets/3597603/d5a82353-ed8e-4260-8cc2-3ee4671050d7)
![2024-05-05 20_40_03-Stillingar – Home Assistant — Mozilla Firefox](https://github.com/gardart/veitur-meter-reader/assets/3597603/893d6719-a354-489a-a428-cb6a5490615e)
![2024-05-05 20_40_34-Stillingar – Home Assistant — Mozilla Firefox](https://github.com/gardart/veitur-meter-reader/assets/3597603/abb68a91-36eb-45f8-bbd5-2639a7b8075f)
![2024-05-05 20_40_55-Stillingar – Home Assistant — Mozilla Firefox](https://github.com/gardart/veitur-meter-reader/assets/3597603/f0b305c1-6d7c-41f8-951d-ae2c305df011)
![2024-05-05 20_41_25-Stillingar – Home Assistant — Mozilla Firefox](https://github.com/gardart/veitur-meter-reader/assets/3597603/3d98d6e9-c1a3-4ac3-8c7d-c0e68adc0203)

# Fjarlægja request rofann úr breytingaferlinum (logbook)
Þar sem rofinn fer í gang á 5 sek fresti þá tekur það mikið pláss í breytingarferlinum (logbook). Hægt að fjarlægja með því að setja inn skilgreiningu fyrir recorder í configuration.yaml

```yaml
recorder:
  exclude:
    entity_globs:
      - switch.veitur_meter_*
```
