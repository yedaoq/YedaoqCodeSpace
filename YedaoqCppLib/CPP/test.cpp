void test()
{
	int i;
	switch(i)
	{
	case 0:
		int x = 3;
		char buf[100];
		strcpy(buf,"may be in debug mode!");
		break;
	default:
		char buf[100];
		strcpy(buf, "should be in release mode!");
		break;
	}
}