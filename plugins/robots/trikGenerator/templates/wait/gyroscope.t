while (!(brick.encoder("@@PORT@@") @@SIGN@@ @@TACHO_LIMIT@@)) {
	brick.wait(10);
}
