import { readLines } from "https://deno.land/std/io/mod.ts";

if (Deno.args.length != 2)
	throw new Error("No args")
const cport = Number(Deno.args[0])
const sport = Number(Deno.args[1])

const encoder = new TextEncoder();

const upstream = await Deno.connect({ port: cport });
const server = Deno.listen({ port: sport });

console.log("Listening on port", sport)

async function proxy(prefix: string, from: Deno.Conn, to: Deno.Conn) {
	for await (const line of readLines(from)) {
		console.log(prefix, line);
		to.write(encoder.encode(line + "\r\n"));
	}
}

for await (const downstream of server)
{
	proxy("<-", downstream, upstream)
	proxy("->", upstream, downstream)
}
  