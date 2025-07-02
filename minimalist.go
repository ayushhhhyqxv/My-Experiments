package main

import "fmt"

type paymenter interface {
	pay(amount float32)
	// further refund,bill method can be added 
}


type payment struct{
	gateway paymenter
}

func (p payment) makePayment(amount float32) {
	// newPayment := razorpay{}
	// newPayment.pay(amount)
	p.gateway.pay(amount)
}

type razorpay struct{}

type stripe struct{}

type test struct{}

type paypal struct{}

func (r razorpay) pay(amount float32) {
	// write own actual logic with apis
	fmt.Println("Making Payment via Razorpay",amount)
}

func (s stripe) pay(amount float32) {
	// write own actual logic with apis
	fmt.Println("Making Payment via Stripe",amount)
}

func (t test) pay(amount float32) {
	// write own actual logic with apis
	fmt.Println("Making Payment via Testing acc",amount)
}

func (pp paypal) pay(amount float32){
	fmt.Println("Making Payment via Paypal",amount)
}

func main() {
	// stripeGW := stripe{}
	// razorpayGW := razorpay{}
	// faketestGW := test{} 
	 paypalGW := paypal{}

	yourPayment := payment{
		gateway: paypalGW,
	}
	yourPayment.makePayment(300)
}