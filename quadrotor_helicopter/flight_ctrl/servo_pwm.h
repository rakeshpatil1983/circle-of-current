static volatile unsigned char next_mask;
static volatile unsigned char chan;

ISR(TIMER1_COMPA_vect) // timer 1 output compare A interrupt
{
	servo_port = next_mask; // new channel output
	unsigned long t = (OCR1A + servo_data.servo_ticks[chan]) & 0xFFFF; // calculate next alarm considering overflow
	OCR1A = t; // set next alarm
	servo_data.safe_to_update_servo_array = 0;
	chan++; // next channel
	switch(chan)
	{
		case f_mot_chan:
		next_mask = (servo_port & (0xFF ^ (_BV(f_motor_pin) | _BV(b_motor_pin) | _BV(l_motor_pin) | _BV(r_motor_pin) | _BV(aux_servo_pin)))) | _BV(b_motor_pin);
		servo_data.servo_new_period_started = 1;
		break;
		case b_mot_chan:
		next_mask = (servo_port & (0xFF ^ (_BV(f_motor_pin) | _BV(b_motor_pin) | _BV(l_motor_pin) | _BV(r_motor_pin) | _BV(aux_servo_pin)))) | _BV(l_motor_pin);
		break;
		case l_mot_chan:
		next_mask = (servo_port & (0xFF ^ (_BV(f_motor_pin) | _BV(b_motor_pin) | _BV(l_motor_pin) | _BV(r_motor_pin) | _BV(aux_servo_pin)))) | _BV(r_motor_pin);
		break;
		case r_mot_chan:
		next_mask = (servo_port & (0xFF ^ (_BV(f_motor_pin) | _BV(b_motor_pin) | _BV(l_motor_pin) | _BV(r_motor_pin) | _BV(aux_servo_pin)))) | _BV(aux_servo_pin);
		servo_data.safe_to_update_servo_array = 1;
		break;
		case aux_servo_chan:
		next_mask = (servo_port & (0xFF ^ (_BV(f_motor_pin) | _BV(b_motor_pin) | _BV(l_motor_pin) | _BV(r_motor_pin) | _BV(aux_servo_pin))));
		servo_data.safe_to_update_servo_array = 1;
		break;
		case 6:
		next_mask = (servo_port & (0xFF ^ (_BV(f_motor_pin) | _BV(b_motor_pin) | _BV(l_motor_pin) | _BV(r_motor_pin) | _BV(aux_servo_pin)))) | _BV(f_motor_pin);
		chan = 0;
	}
}