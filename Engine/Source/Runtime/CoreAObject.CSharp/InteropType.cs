// Copyright 2020-2023 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.Reflection;
using System.Runtime.InteropServices;

namespace AE.CoreAObject;

public class InteropType : Type
{
    private readonly Type managedType;
    private readonly IntPtr nativePointer;

    public InteropType(Type managedType, IntPtr nativePointer)
    {
        this.managedType = managedType;
        this.nativePointer = nativePointer;
    }

    public override Assembly Assembly => managedType.Assembly;

    public override string? AssemblyQualifiedName => managedType.AssemblyQualifiedName;

    public override Type? BaseType => managedType.BaseType;

    public override string? FullName => managedType.FullName;

    public override Guid GUID => managedType.GUID;

    public override Module Module => managedType.Module;

    public override string? Namespace => managedType.Namespace;

    public override Type UnderlyingSystemType => managedType.UnderlyingSystemType;

    public override string Name => managedType.Name;

    public override ConstructorInfo[] GetConstructors(BindingFlags bindingAttr)
    {
        return managedType.GetConstructors(bindingAttr);
    }

    public override object[] GetCustomAttributes(bool inherit)
    {
        return managedType.GetCustomAttributes(inherit);
    }

    public override object[] GetCustomAttributes(Type attributeType, bool inherit)
    {
        return managedType.GetCustomAttributes(attributeType, inherit);
    }

    public override Type? GetElementType()
    {
        return managedType.GetElementType();
    }

    public override EventInfo? GetEvent(string name, BindingFlags bindingAttr)
    {
        return managedType.GetEvent(name, bindingAttr);
    }

    public override EventInfo[] GetEvents(BindingFlags bindingAttr)
    {
        return managedType.GetEvents(bindingAttr);
    }

    public override FieldInfo? GetField(string name, BindingFlags bindingAttr)
    {
        return managedType.GetField(name, bindingAttr);
    }

    public override FieldInfo[] GetFields(BindingFlags bindingAttr)
    {
        return managedType.GetFields(bindingAttr);
    }

    [return: DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.Interfaces)]
    public override Type? GetInterface(string name, bool ignoreCase)
    {
        return managedType.GetInterface(name, ignoreCase);
    }

    public override Type[] GetInterfaces()
    {
        return managedType.GetInterfaces();
    }

    public override MemberInfo[] GetMembers(BindingFlags bindingAttr)
    {
        return managedType.GetMembers(bindingAttr);
    }

    public override MethodInfo[] GetMethods(BindingFlags bindingAttr)
    {
        return managedType.GetMethods(bindingAttr);
    }

    public override Type? GetNestedType(string name, BindingFlags bindingAttr)
    {
        return managedType.GetNestedType(name, bindingAttr);
    }

    public override Type[] GetNestedTypes(BindingFlags bindingAttr)
    {
        return managedType.GetNestedTypes(bindingAttr);
    }

    public override PropertyInfo[] GetProperties(BindingFlags bindingAttr)
    {
        return managedType.GetProperties(bindingAttr);
    }

    public override object? InvokeMember(string name, BindingFlags invokeAttr, Binder? binder, object? target, object?[]? args, ParameterModifier[]? modifiers, CultureInfo? culture, string[]? namedParameters)
    {
        return managedType.InvokeMember(name, invokeAttr, binder, target, args, modifiers, culture, namedParameters);
    }

    public override bool IsDefined(Type attributeType, bool inherit)
    {
        return managedType.IsDefined(attributeType, inherit);
    }

    protected override TypeAttributes GetAttributeFlagsImpl()
    {
        throw new NotImplementedException();
    }

    protected override ConstructorInfo? GetConstructorImpl(BindingFlags bindingAttr, Binder? binder, CallingConventions callConvention, Type[] types, ParameterModifier[]? modifiers)
    {
        throw new NotImplementedException();
    }

    protected override MethodInfo? GetMethodImpl(string name, BindingFlags bindingAttr, Binder? binder, CallingConventions callConvention, Type[]? types, ParameterModifier[]? modifiers)
    {
        throw new NotImplementedException();
    }

    protected override PropertyInfo? GetPropertyImpl(string name, BindingFlags bindingAttr, Binder? binder, Type? returnType, Type[]? types, ParameterModifier[]? modifiers)
    {
        throw new NotImplementedException();
    }

    protected override bool HasElementTypeImpl()
    {
        throw new NotImplementedException();
    }

    protected override bool IsArrayImpl()
    {
        throw new NotImplementedException();
    }

    protected override bool IsByRefImpl()
    {
        throw new NotImplementedException();
    }

    protected override bool IsCOMObjectImpl()
    {
        throw new NotImplementedException();
    }

    protected override bool IsPointerImpl()
    {
        throw new NotImplementedException();
    }

    protected override bool IsPrimitiveImpl()
    {
        throw new NotImplementedException();
    }

    [DllImport("CoreAObject", EntryPoint = "NativeCall_CoreAObject_Type_Construct", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    internal static extern IntPtr NativeCall_Construct(IntPtr self);
}
