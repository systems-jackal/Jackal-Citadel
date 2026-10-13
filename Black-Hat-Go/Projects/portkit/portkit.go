package portkit

func IsValid(p int) bool {
	return p >= 1 && p <= 65535
}

func IsWellKnown(p int) bool {
	return IsValid(p) && p <= 1023
}

func Classify(p int) string {
	if !IsValid(p) {
		return "Invalid"
	}

	if p <= 1023 {
		return "Well Known"
	}

	if p <= 49151 {
		return "Registerd"
	}

	return "Dynamic"
}

func ServiceName(p int) string {
	switch p {
	case 21:
		return "ftp"
	case 22:
		return "ssh"
	case 2222:
		return "ssh-alt" //In this case the alt mean alternative.
	case 53:
		return "dns"
	case 80:
		return "http"
	case 443:
		return "https"
	case 3306:
		return "mysql"
	case 8080:
		return "http-alt"
	}

	return "Unknown"
}
