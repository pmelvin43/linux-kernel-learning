#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

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



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xb6c08e4c, "class_create" },
	{ 0xf350d701, "device_create" },
	{ 0x52b15b3b, "__unregister_chrdev" },
	{ 0x30a11079, "device_destroy" },
	{ 0xfd4b4a36, "class_destroy" },
	{ 0xd272d446, "__fentry__" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0xd272d446, "__put_user_1" },
	{ 0xe8213e80, "_printk" },
	{ 0xdd6830c7, "sprintf" },
	{ 0x97c7a1f1, "__register_chrdev" },
	{ 0x814e12e5, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xb6c08e4c,
	0xf350d701,
	0x52b15b3b,
	0x30a11079,
	0xfd4b4a36,
	0xd272d446,
	0xd272d446,
	0xd272d446,
	0xe8213e80,
	0xdd6830c7,
	0x97c7a1f1,
	0x814e12e5,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"class_create\0"
	"device_create\0"
	"__unregister_chrdev\0"
	"device_destroy\0"
	"class_destroy\0"
	"__fentry__\0"
	"__x86_return_thunk\0"
	"__put_user_1\0"
	"_printk\0"
	"sprintf\0"
	"__register_chrdev\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "3F54E868DF647088B920D32");
