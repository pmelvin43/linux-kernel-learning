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
	{ 0xcec7a145, "debugfs_create_u32" },
	{ 0xfe4ce47a, "debugfs_create_bool" },
	{ 0xe8213e80, "_printk" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0x74fc8c8a, "debugfs_remove" },
	{ 0x4d8419c6, "param_ops_bool" },
	{ 0x4d8419c6, "param_ops_uint" },
	{ 0xd272d446, "__fentry__" },
	{ 0x2379af1d, "debugfs_create_dir" },
	{ 0x814e12e5, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xcec7a145,
	0xfe4ce47a,
	0xe8213e80,
	0xd272d446,
	0x74fc8c8a,
	0x4d8419c6,
	0x4d8419c6,
	0xd272d446,
	0x2379af1d,
	0x814e12e5,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"debugfs_create_u32\0"
	"debugfs_create_bool\0"
	"_printk\0"
	"__x86_return_thunk\0"
	"debugfs_remove\0"
	"param_ops_bool\0"
	"param_ops_uint\0"
	"__fentry__\0"
	"debugfs_create_dir\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "5D6C4935643707C7E44181B");
