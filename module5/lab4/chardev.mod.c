#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x8a7d75b0, "module_layout" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x25ac39cd, "class_destroy" },
	{ 0xf4622094, "device_destroy" },
	{ 0xeff3c5b7, "device_create" },
	{ 0x5448da20, "__class_create" },
	{ 0x92997ed8, "_printk" },
	{ 0x3c6769e7, "__register_chrdev" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0xb788fb30, "gic_pmr_sync" },
	{ 0x31909811, "cpu_hwcap_keys" },
	{ 0x14b89635, "arm64_const_caps_ready" },
	{ 0x1e49e99f, "cpu_hwcaps" },
	{ 0xdcb764ad, "memset" },
	{ 0x4b0a3f52, "gic_nonsecure_priorities" },
	{ 0x4cf93c47, "try_module_get" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xe4b5fe4e, "module_put" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "B7A65889439BA42BF828E08");
