# ONDA

Project ONDA aims to develop a real-time, handheld musical instrument tuner, using a Raspberry Pi 3.

## Overview

This project uses a Raspberry Pi 3 in conjunction with custom made hardware and software, to process audio data and output frequency information to the user. A Raspberry Pi HAT audio card based on the Cirrus Logic WM8731 codec has been designed, capable of sampling audio data at rates of up to 96KHz. An LCD to display the frequency data is also featured. Software has been developed which is capable of both sampling PCM audio data, and calculating the FFT of this data. The frequency information gained from the FFT analysis is used to send appropriate tuning messages to the LCD.

<!---
## Installation

```python
import foobar

foobar.pluralize('word') # returns 'words'
foobar.pluralize('goose') # returns 'geese'
foobar.singularize('phenomena') # returns 'phenomenon'
```
--->


## Contributing
We welcome pull requests. For major changes, please open an issue first to discuss what you would like to change.


## Social Media

[Instagram](https://www.instagram.com/ondasounds/ "Instagram") -------> Follow us on Instagram for the most frequent updates from project ONDA

[Facebook](https://www.instagram.com/ondasounds/ "Facebook") -------> Like us on Facebook
